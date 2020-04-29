/* Socket tutorial

url: http://www.linuxhowtos.org/C_C++/socket.htm

*/
/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <math.h>

#define NUM_OF_CONNECTIONS 4

double central_temp;
double old_central_temp;

typedef struct handle_connection_args
{
	double local_temp;
	int sock_id;
	int process_id;
}CONNECTION_ARGS;

void *handle_connection(void *); //retrieves, updates, and sends temperatures to and from the external processes
void *final_message(void *); //Sends the final message signaling the external processes that the system has stabilized 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	 CONNECTION_ARGS *connections = malloc(NUM_OF_CONNECTIONS*sizeof(struct handle_connection_args)); //allocates an array of size NUM_OF_CONNECTIONS for each connections data
	 CONNECTION_ARGS temp;
	 int connection_count = 0; //counts the number of current connections to wait until all connections are made
	 int flag = 0; //flag used to signal when system is stabilized
	 int sorted = 1; //used to initially sort the process ids
     int sockfd, newsockfd, portno, pid;
	 double past_temps[NUM_OF_CONNECTIONS]; //holds past external temperatures
	 double curre_temps[NUM_OF_CONNECTIONS]; //holds current external temperatures
	 double sum; // used to sum external temperatures and calculate new centeral temperatures
     socklen_t clilen;	
     struct sockaddr_in serv_addr, cli_addr;
	 pthread_t thread_connections[NUM_OF_CONNECTIONS]; //creates an array of size NUM_OF_CONNECTIONS for each thread that is made
	 
	 
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }else if(argc < 3)
	 {
		 fprintf(stderr,"ERROR, no central temp provided\n");
         exit(1);
	 }
	 
	 central_temp = strtod(argv[2], NULL);
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
			  sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
	 

     while (connection_count<NUM_OF_CONNECTIONS) //loops and waits until all the connections are made
	 {
		connections[connection_count].sock_id = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		connection_count++;
     }
	
	 while(1) //Loop that repeatedly writes and reads data to and from the external processes
	 {
		 for(int i = 0; i<NUM_OF_CONNECTIONS; i++) //calls handle_connection for each external process
		 {
			pthread_create(&thread_connections[i], NULL, handle_connection, &connections[i]);
		 }
		 
		 flag = 1;
		 printf("\n"); //Necessary to flush the buffer, otherwise output is incorrect
		 if(sorted){
			 for(int i = 0; i<NUM_OF_CONNECTIONS; i++)//loop that sorts the processes by their id so that processes are printed in order from 1 to max NUM_OF_CONNECTIONS
			 {
				temp = connections[i];
				for(int j = 0; j<NUM_OF_CONNECTIONS; j++)
				{
					if(connections[j].process_id == i+1)
					{
						connections[i] = connections[j];
						connections[j] = temp;
					}
				}
			 }
			 sorted = 0;
		 }
		 
		 //Output segment to print all current temperatures
		 printf("Central Temperature: %.4lf\n", central_temp);
		 printf("External Temperatures: \n");
		 for(int i = 0; i<NUM_OF_CONNECTIONS; i++)
		 {
			 printf("Process %d: %.4lf\n", connections[i].process_id, connections[i].local_temp);
			 curre_temps[i] = connections[i].local_temp;
		 }
		 
		 //loop to check if system is stabilized
		 for(int i = 0; i<NUM_OF_CONNECTIONS; i++)
		 {
			if(curre_temps[i] != past_temps[i] || old_central_temp != central_temp)
			{
				flag = 0;
				break;
			}
		 }
		 
		 //sums external temperatures for new centeral temperature computation
		 sum = 0;
		 for(int i = 0; i<NUM_OF_CONNECTIONS; i++)
		 {
			 sum += curre_temps[i];
			 past_temps[i] = curre_temps[i];
			 pthread_join(thread_connections[i], NULL);
		 }
		 
		 //if the flag has been set, sends the final message to every external process to signal stabilization and break the loop otherwise, the loop continues to execute
		 if(flag)
		 {
			for(int i = 0; i<NUM_OF_CONNECTIONS; i++)
			{
				pthread_create(&thread_connections[i], NULL, final_message, &connections[i]);
			}
			
			for(int i = 0; i<NUM_OF_CONNECTIONS; i++)
			{
				pthread_join(thread_connections[i], NULL);
			}

			printf("System has Stabilized\n\n");
			break;
		 }else{
			printf("System is Unstable\n\n");
		 }
		 old_central_temp = central_temp; //saves the current central temp to the old so that stabilization may be checked next iteration
		 central_temp = round(((2*central_temp + sum)/6)*10000)/10000; //computes the new central temp and rounds it to 4 precision points
	 }
     close(sockfd);
	 free(connections);
     return 0; 
}		

void *handle_connection(void *void_sock) //retrieves, updates, and sends temperatures to and from the external processes
{
   int n;
   int length = 0;
   int sock = ((struct handle_connection_args*)void_sock)->sock_id;
   char buffer[256];
   char *token; //used to break up the message from the external processes to save the new data
   double temperature = 0;
   int id = 0;
   
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");

   token = strtok(buffer, " ");
   temperature = strtod(token, NULL);
   token = strtok(NULL, " ");
   id = atoi(token);
   
   ((struct handle_connection_args*)void_sock)->local_temp = temperature; //saves updated temperature for the current socket
   ((struct handle_connection_args*)void_sock)->process_id = id; //saves process id of the current socket
   
   bzero(buffer,256);
   snprintf(buffer, 256, "%lf", central_temp); //loads central temp into buffer to send to the external process on the socket
   strcat(buffer, " NotStable"); //signals to the socket that the system isn't stable
   n = write(sock, buffer,255);
   if (n < 0) error("ERROR writing to socket");
}

void *final_message(void *void_sock) //Sends the final message signaling the external processes that the system has stabilized 
{
   int n;
   int sock = ((struct handle_connection_args*)void_sock)->sock_id; //selects which process to send the message to 
   char buffer[256];
   double temperature = 0;
   
   temperature = ((struct handle_connection_args*)void_sock)->local_temp; //loads the most recent external temperature from the current socket into buffer 
   bzero(buffer,256);
   snprintf(buffer, 256, "%lf", temperature);
   strcat(buffer, " Stable"); //appends stable to the buffer as a signal that the system is stabilized
   n = write(sock, buffer,255);
   if (n < 0) error("ERROR writing to socket");
}
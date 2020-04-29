/* Socket tutorial

url: http://www.linuxhowtos.org/C_C++/socket.htm

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	// similar to server's 
    int sockfd, portno, n;
	char temperature[254];
	char id[2];
	char central_temp[253];
	double central_temp_float;
	double new_temperature;
	double temperature_float;
	int flag = 0;
	char *token;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 5) {
       fprintf(stderr,"usage %s hostname port initialtemp id\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
		
		
		
		
		
	// The variable argv[1] contains the name of a host on the Internet	
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	
	
	// similar to server's 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
	
	// Establish a connection to the server. It takes three arguments, 
	// the socket file descriptor, the address of the host to which it wants 
	// to connect (including the port number), and the size of this address. 
	// The client needs to know the port number of the server, but it does not 
	// need to know its own port number. This is typically assigned by the 
	// system when connect is called.
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

	//reads command line args of temperature and id and loads them into the buffer to send to the central process
	strcpy(temperature, argv[3]);
	strcpy(id, argv[4]);

	bzero(buffer,256);
	strcpy(buffer, temperature);
	strcat(buffer, " ");
	strcat(buffer, id);
	strcat(buffer, "\0");
	n = write(sockfd,buffer,strlen(buffer));
	if (n < 0)error("ERROR writing to socket");

	while(1)//loop that repeatedly reads from the server, if a message is recieved, the if is executed, otherwise the process continues to lsiten
	{
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		if(!(strcmp(buffer,"\0")==0))
		{
			//breaks up message from central process into the temperature and the signal of if the system is stable
			token = strtok(buffer, " ");
			strcpy(central_temp, token);
			central_temp_float = strtod(token, NULL);
			token = strtok(NULL, " "); 
			
			//if the system is stable prints most recent local temperature and final message then breaks the loop
			if(strcmp(token, "Stable") == 0)
			{
				printf("System Stabilized\nFinal Temperature: %.4lf\n", central_temp_float);
				break;		
			}
			
			//reads the new central temp, computes the new external temperature rounded to 4 precision points, and converts it to a string
			temperature_float = strtod(temperature,NULL); //
			new_temperature = round((((temperature_float*3)+(2*central_temp_float))/5)*10000)/10000; //
			snprintf(temperature, 254, "%lf", new_temperature);
			
			//loads new external temperature into the buffer and sends it to the central process 
			bzero(buffer,256);
			strcpy(buffer, temperature);
			strcat(buffer," ");
			strcat(buffer, id);
			n = write(sockfd,buffer,strlen(buffer));
			if (n < 0) error("ERROR writing to socket");

		}
	}
		
    close(sockfd);
    return 0;
}

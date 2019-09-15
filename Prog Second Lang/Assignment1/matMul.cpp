#include <iostream>
#include <fstream>
#include <string>
#include <new>
using namespace std;

void multiply(int size, int **array_1, int ** array_2); //accepts two arrays of pointers and a size variable to perform the multiplication

int main(int argc, char *argv[]){
	string line; //variable used to read file
	
	ifstream mat_file(argv[1]); // This block reads the 
	getline(mat_file, line);	// first line of the file
	int mat_size=stoi(line);	// to find out array size
	
	int **array_1; //matrix 1
	int **array_2; //matrix 2

	array_1 = new int*[mat_size]; //allocates neccessary space for array 1
	for(int i=0; i<mat_size; i++){
		array_1[i] = new int[mat_size];
	}
	
	array_2 = new int*[mat_size]; //allocates neccessary space for array 2
	for(int i=0; i<mat_size; i++){
		array_2[i] = new int[mat_size];
	}
	
	for(int i=0; i<mat_size; i++){ //reads file values into array_1
		for(int j=0; j<mat_size; j++){
			getline(mat_file, line);
			array_1[i][j]=stoi(line);
		}
	}
	
	for(int i=0; i<mat_size; i++){ //reads file values into array_2
		for(int j=0; j<mat_size; j++){ 
			getline(mat_file, line); 
			array_2[i][j]=stoi(line); 
		}
	}
	multiply(mat_size, array_1, array_2);
	
	delete array_1; //frees the space allocated for array_1
	delete array_2; //frees the space allocated for array_2
}

void multiply(int size, int **array_1, int ** array_2){
	int answers[size][size]; //array that holds answers
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			answers[i][j]=0; //sets positions value to 0 to prevent junk values in memory from interfering
			for(int k=0; k<size; k++){
				answers[i][j]+=(array_1[i][k])*(array_2[k][j]); 
			}
			cout << answers[i][j] << " ";
		}
		cout << endl;
	}
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "matrix.h"

using namespace std;

int main(int argc, char *argv[]){
	int flag=1; //used to see if an inverse exists
	matrix mat(argv[2]); //builds matrix from file
	flag=mat.determinant(); //flag value to see if determinant indicated existance of inverse
	mat.transpose(); //prints a transposed matrix
	mat.multiplication(stoi(argv[1])); //performs matrix multiplication with input integer
	if(flag!=0){ //checks flag for existance of an inverse
		mat.inverse();
	}else{
		cout <<"Inverse: " << endl << "No Inverse";
	}
	
	//mat.print(); //used in testing
}
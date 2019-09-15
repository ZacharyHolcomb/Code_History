#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "matrix.h"
#include "linear.h"

using namespace std;

linear::linear(string input_file){
	string line; //variable used to hold file info

	/*
	Reads the first two lines of the file to
	find out the matrix size and the nuumber 
	of constant terms
	*/
	
	ifstream data_file(input_file);  
	getline(data_file, line);	
	int mat_size=stoi(line);
	getline(data_file, line);	
	num_of_constants=stoi(line);	
	
	/*
	Builds matrix of coefficients from file
	*/
	for(int i=0; i<mat_size; i++)
	{ 
		mat.push_back(vector<double>());
		for(int j=0; j<mat_size-1; j++)
		{
			getline(data_file, line, ' ');
			mat[i].push_back(stoi(line));
		}
		getline(data_file, line);
		mat[i].push_back(stoi(line));
	}
	
	/*
	Builds the matrix of constants from file
	*/
	
	for(int i=0; i<num_of_constants; i++)
	{
		constants.push_back(vector<int>());
		for(int j=0; j<mat_size; j++)
		{
			getline(data_file, line);
			constants[i].push_back(stoi(line));
			//cout << constants[i][j] << endl;
		}
	}
}

void linear::solve(){
	vector<vector<double>> coefficient_inverses; //matrix to hold inverses of the coefficient matrix
	coefficient_inverses=inverse();
	
	double total=0; // used to accumulate the results for final answer of each row of the inverse matrix for each constant

	/*
	Loops through inverse matrix and multiplies each value in a row to 
	a value in the corresponding row of the constants matrix. All answers
	are accumulated into total variable and printed out with proper
	formatting before the next iteration.
	*/
	
	for(int i=0; i<num_of_constants; i++)
	{
		for(int j=0; j<mat.size(); j++)
		{	
			total=0;
			for(int k=0; k<mat.size(); k++)
			{
				total=total+(coefficient_inverses[j][k]*constants[i][k]);
			}
			printf("%.2lf ", total);
		}
		cout << endl;
	}
}

void linear::print_constants()
{
	/*
	Prints the matrix of constants
	*/
	
	cout<<"Constants:" <<endl;	
	for(int i=0; i<num_of_constants; i++)
	{
		for(int j=0; j<mat.size(); j++)
		{
			cout << constants[i][j] << " ";
		}
		cout << endl;
	}
}	

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "matrix.h"
#include "linear.h"

using namespace std;

int matrix::determinant()
{
	double multiplier=0.0;
	cout <<"Determinant: "<<endl;
    int flag = 1;
    vector<vector<double>> determinate_result = mat; //creates matrix to manipulate for determinate results
	
    for(int i = 0; i < determinate_result[0].size() - 1; i++)//loops through each row
	{
        if(determinate_result[i][i] == 0)
		{
            for(int k = 0; k < determinate_result.size(); k++) //used to swap values around when a 0 data point is hit
			{ 
                double temp = determinate_result[i][k];
                determinate_result[i][k] = determinate_result[i + 1][k];
                determinate_result[i + 1][k] = temp;
            }
            flag = flag * -1;
        }
		
        for(int j = i + 1; j < determinate_result.size(); j++)//performs required computations for each position in the determinate matrix
		{
            multiplier = (determinate_result[j][i] / determinate_result[i][i]); //gets multiplier for current row 
            for(int k = 0; k < determinate_result[0].size(); k++)
			{
                determinate_result[j][k] = determinate_result[j][k]-(determinate_result[i][k] * multiplier); //sets determinate en_result end value for the position
            }
        }
		
    }
    double total = determinate_result[0][0]; //calculates the total to be printed out at the end
    for(int i = 1; i < determinate_result.size(); i++)
	{
        total *= determinate_result[i][i];
    }
	
	if(total!=0)//used for formating determinate result of 0 to avoid -0 being printed
	{ 
		cout << total * flag << endl;
	}else{
		cout << 0 <<endl;
	}
	return total;
}

void matrix::transpose() //prints matrix with row and column index swapped
{
	cout<<"Transpose: " << endl;
	for(int i=0; i<mat.size(); i++){
		for(int j=0; j<mat[i].size(); j++){
			cout<<mat[j][i]<<" ";
		}
		cout << endl;
	}
}

void matrix::multiplication(int mult_factor) //prints matrix with values being multiplied by command line integer
{
	cout<<"Element-wise multiplication:" << endl;
	for(int i=0; i<mat.size(); i++)
	{
		for(int j=0; j<mat[i].size(); j++)
		{
			cout<<mult_factor*mat[i][j]<<" ";
		}
		cout << endl;
	}
}

vector<vector<double>> matrix::inverse()
{
	double multiplier=0;
	double divisor=0;
	int value=0;// used as a temp variable
	//cout<<"Inverse: " << endl;
    vector<vector<double>> temp = mat; //copy of matrix manipulated when computing inverse
    vector<vector<double>> inverse_result = mat; //matrix used to print results 
	
    for(int i = 0; i < temp.size(); i++) //creates secondary matrix of diagonal 1's used in inverse computation
	{ 
        for(int j = 0; j < temp[0].size(); j++)
		{
			if(i==j)
			{
				inverse_result[i][j] = 1;
			}else
			{
				inverse_result[i][j]=0;
			}
        }
    }
	
    for(int i = 0; i < temp.size(); i++)
	{ 
        if(temp[i][i] == 0)	//swaps current oposition and right neighbor's value if a 0 is encountered on a diagonal
		{
            for(int k = 0; k < temp.size(); k++)
			{
				value = temp[i][k];	//swaps values in copy of matrix
                temp[i][k] = temp[i + 1][k];
                temp[i + 1][k] = value;
				
                value = inverse_result[i][k]; //swaps values in matrix of diagonal 1's
                inverse_result[i][k] = inverse_result[i + 1][k];
                inverse_result[i + 1][k] = value;
            }
        }
		
        divisor = temp[i][i]; //used to divide column values by current row
        for(int k = 0; k < temp.size(); k++)
		{
            temp[i][k] = temp[i][k]/divisor;
            inverse_result[i][k] = inverse_result[i][k]/divisor;
        }
		
        for(int j = 0; j < temp[0].size(); j++)
		{
            if(i != j) { //avoids hitting the same position when accessing temp[i][k] and temp[j][k]
                multiplier = temp[j][i];
                for(int k = 0; k < temp.size(); k++) //computes the inverse fo each position
				{
                    temp[j][k] = (temp[j][k] - multiplier * temp[i][k]);
                    inverse_result[j][k] = (inverse_result[j][k] - multiplier * inverse_result[i][k]);
                }
            }
        }
    }
	/*
    for(int i=0; i<inverse_result.size(); i++) //prints out results of the inverse of the matrix
	{
		for(int j=0; j<inverse_result[i].size(); j++){
			cout<<inverse_result[i][j]<<" ";
		}
		cout << endl;
	}*/
	return inverse_result;
}

void matrix::print()//used in testing, prints out matrix
{
	cout<<"Matrix: " << endl;
	for(int i=0; i<mat.size(); i++)
	{
		for(int j=0; j<mat[i].size(); j++)
		{
			cout<<mat[i][j]<<" ";
		}
		cout << endl;
	}
}
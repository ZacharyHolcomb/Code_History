#ifndef matrix_h
#define matrix_h
#include <vector>

using namespace std;
/*
Class file for matrix class
*/
class matrix
{
	public:
		vector<vector<double>> mat;
		void print();
		int determinant();
		void transpose();
		void multiplication(int mult_factor);
		vector<vector<double>> inverse();
};

#endif
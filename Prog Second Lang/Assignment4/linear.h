#ifndef linear_h
#define linear_h
#include <vector>
#include "matrix.h"

using namespace std;
/*
Class file for linear equation subclass of matrix
*/
class linear: public matrix
{
	public:
		linear(string input_file);
		vector<vector<int>> constants;
		void solve();
		void print_constants();
		int num_of_constants;
};

#endif
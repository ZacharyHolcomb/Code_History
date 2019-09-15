#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "matrix.h"
#include "linear.h"

using namespace std;

int main(int argc, char *argv[])
{	
	linear equation(argv[1]); //builds matrix from file
	equation.solve();
	//equation.print(); //prints matrix of coefficients, used for testing
	//equation.print_constants(); //prints matrix of constant terms, used for testing
}
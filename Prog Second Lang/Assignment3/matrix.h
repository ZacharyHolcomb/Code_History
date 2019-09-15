#include <vector>
using namespace std;
//Class file holding 2D vector
class matrix{
	public:
		vector<vector<double>> mat;
		matrix(string file_name);
		void print();
		int determinant();
		void transpose();
		void multiplication(int mult_factor);
		void inverse();
};
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void read_files(string, string, vector<vector<int>>, vector<int>, vector<vector<int>>); //Function to read files and call functions to make calculations
vector<double> build_profile(vector<vector<int>>, vector<int>); //function to calculate weight values
void profile_input_data(vector<double>, vector<vector<int>>); //Function to calculate output based off of previously found weights
void print_vectors(vector<vector<int>>, vector<int>, vector<vector<int>>); //Function used for testing purposes

int main(int argc,char *argv[])
{ 
	vector<int> training_expected_output; //vector for expected output to determine if the weights are correct
	vector<vector<int>> output_calculation_vector; //vector for input data to calculate output
	vector<vector<int>> training_vector; //vector for input data to calculate weights

	read_files(argv[1], argv[2], training_vector, training_expected_output, output_calculation_vector);

	return 0;
}

void read_files(string training_file, string test_file, vector<vector<int>> training_vector,vector<int> training_expected_output,vector<vector<int>> output_calculation_vector)
{
	string line; //variable to hold current line of files
	int num_lines_training = 0; //Reads first line of the training file to figure out how many lines there are
	int num_lines_testing = 0; //Reads first line of the output calculation file to figure out how many lines there are
	vector<double> weights; //Vector holding weights returned by build_profile
	vector<double> weights_copy; //Copy of weights that are sent to profile_input_data for calculating outcome

	/* Reads the file holding vector information used
	in the training set for the data. 1's are also 
	appended to the left column of the vector for X0 
	value's in the formula.Also saves the expected 
	outputs for training set.*/

	ifstream training_set(training_file);
	getline(training_set, line);
	num_lines_training=stoi(line);
	

	for (int i = 0; i < num_lines_training; i++)
	{
		training_vector.push_back(vector<int>());
		for (int k = 0; k < 5; k++)
		{
			if(k == 0) 
			{
				training_vector[i].push_back(1);
			}
			else
			{
				getline(training_set, line, ' ');
				training_vector[i].push_back(stoi(line));
			}
		}
		getline(training_set, line);
		training_expected_output.push_back(stoi(line));
	}

	training_set.close();
	
	/* Reads the file holding vector information used
	in the output calculation's for the data. 1's are 
	also appended to the left column of the vector for 
	X0 value's in the formula*/

	ifstream test_set(test_file);
	getline(test_set, line);
	num_lines_testing=stoi(line);

	for (int i = 0; i < num_lines_testing; i++)
	{
		output_calculation_vector.push_back(vector<int>());
		for (int k = 0; k < 5; k++)
		{	
			if(k == 0)
			{
				output_calculation_vector[i].push_back(1);
			}
			else
			{
				if(k!=4)
				{
					getline(test_set, line, ' ');
					output_calculation_vector[i].push_back(stoi(line));
				}
				else
				{
					getline(test_set, line, '\n');
					output_calculation_vector[i].push_back(stoi(line));
				}
			}
		}
	}

	test_set.close();

	weights = build_profile(training_vector, training_expected_output);
	/*Copies all the weights into the weights_copy vector*/
	for (int i = 0; i < 5; i++)
	{
		weights_copy.push_back(weights[i]);
	}
	profile_input_data(weights_copy, output_calculation_vector);
}


vector<double> build_profile(vector<vector<int>> training_vector, vector<int> training_expected_output)
{
	int flag = 1; //used to signal epochs are done
	int outcome = 0; //outcome for the current calculation
	double total = 0; //sum of the current calculation
	double alpha_val = 0.5; //learning rate for the formula
	vector<double> weights; //vector to hold weights, initialized with 0's
	
	for (int i = 0; i < 4; i++)
	{
		weights.push_back(0);
	}

	/*Does 50 epochs and adjusts the weights until the result
	is the expected outcome. Once the proper weights have been
	found the loops break out.*/

	for (int i = 0; i < 50; i++) 
	{
		flag = 1;
		for (int k = 0; k < training_vector.size(); k++)
		{
			total = 0;
			for (int j = 0; j < 5; j++)
			{
				total += weights[j] * training_vector[k][j];
			}

			if (total > 0)
			{
				outcome = 1;
			}
			else
			{
				outcome = 0;
			}

			for (int j = 0; j < 5; j++)
			{
					weights[j] += alpha_val * (training_expected_output[k] - outcome)*training_vector[k][j];
			}
		}

		if (flag == 1)
		{
			break;
		}
	}
		return weights;
}

void profile_input_data(vector<double> weights_copy, vector<vector<int>> output_calculation_vector)
{
	vector<int> outputs; //saves outputs to be printed later
	int total = 0; //used to calculate totals used to determine outputs

	/*Uses weights clculated earlier with values
	from output_calculation_vector to calculate
	the output of the input */

	for (int i = 0; i < output_calculation_vector.size(); i++)
	{
		total = 0;
		for (int j = 0; j < 5; j++)
		{
			total += weights_copy[j] * output_calculation_vector[i][j];
		}
		if (total > 0) 
		{
			outputs.push_back(1);
		}
		else
		{
			outputs.push_back(0);
		}
	}
	
	/*Outputs the outcome of the calculations*/

	for (int i = 0; i < outputs.size(); i++)
	{
		cout << outputs[i] << " ";
	}
}

void print_vectors(vector<vector<int>> training_vector, vector<int> training_expected_output, vector<vector<int>> output_calculation_vector)
{

	/*Function used for testing purposes to print different vectors*/

	for (int i = 0; i < training_vector.size(); i++)
	{
		for (int k = 0; k < 5; k++)
		{
			cout << training_vector[i][k] << " ";
		}
		cout << "\n";
	}

	cout << "\n";

	for (int i = 0; i < output_calculation_vector.size(); i++)
	{
		for (int k = 0; k < 5; k++)
		{
			cout << output_calculation_vector[i][k] << " ";
		}
		cout << "\n";
	}

	cout << "\n";

	for (int i = 0; i < training_expected_output.size(); i++)
	{
		cout << training_expected_output[i]<< " ";
	}
}
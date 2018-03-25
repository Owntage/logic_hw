//
// Created by Owntage on 3/25/2018.
//

#include <iostream>
#include <fstream>
#include "prop_input_parser.h"
#include "propositional_proof.h"

using namespace std;

int main()
{
	string input_filename;
	string output_filename;

	cout << "enter input filename: " << endl;
	cin >> input_filename;

	cout << "enter output filename: " << endl;
	cin >> output_filename;

	string input;
	string targetExpr;
	vector<string> assumptions;
	vector<string> proofExpressions;

	ifstream input_f(input_filename);
	if (input_f)
	{
		while (!input_f.eof())
		{
			input.push_back(input_f.get());
		}
	}

	parsePropositionalInput(input, assumptions, proofExpressions, targetExpr);

	std::cout << "target expression: " << targetExpr;
	return 0;
}
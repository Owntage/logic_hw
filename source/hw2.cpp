//
// Created by Owntage on 3/25/2018.
//

#include <iostream>
#include <fstream>
#include "prop_input_parser.h"
#include "deduction.h"

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
	auto deductiveProof = buildDeductiveProof(assumptions, proofExpressions);
	ofstream output_f(output_filename);
	
	for (int i = 0; i < assumptions.size() - 1; i++)
	{
		output_f << assumptions[i];
		if (i != assumptions.size() - 2)
		{
			output_f << ",";
		}
	}
	output_f << "|-" << assumptions.back() << "->" << targetExpr << endl;
	
	for (int i = 0; i < deductiveProof.size(); i++)
	{
		output_f << deductiveProof[i] << endl;
	}

	return 0;
}
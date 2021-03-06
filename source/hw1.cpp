//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include <vector>
#include <fstream>
#include "propositional_proof.h"
#include "prop_input_parser.h"

using namespace std;

typedef std::string::iterator str_it;

template<typename T>
void printVector(std::vector<T> vector)
{
	for(int i = 0; i < vector.size(); i++)
	{
		std::cout << vector[i] << std::endl;
	}
}

int main()
{
	std::cout << "enter input filename: " << endl;
	std::string filename;
	std::cin >> filename;

	std::cout << "enter output filename: " << endl;
	std::string output_filename;
	std::cin >> output_filename;

	std::string input;
	ifstream input_f(filename);
	if (input_f)
	{
		while (!input_f.eof())
		{
			input.push_back(input_f.get());
		}
	}

	std::vector<string> assumptions;
	std::vector<string> proofExpressions;
	std::string targetExpr;

	parsePropositionalInput(input, assumptions, proofExpressions, targetExpr);


	PropositionalProofChecker proofChecker(assumptions, proofExpressions);

	ofstream output_f(output_filename);
	for (int i = 0; i < proofChecker.result.size(); i++)
	{
		std::string exprResult;
		auto resMap = proofChecker.result[i];
		switch (resMap[EXPR_TYPE_KEY])
		{
			case EXPR_AXIOM:
			{
				exprResult += u8"Сх. акс. " + to_string(resMap[EXPR_AXIOM_KEY]);
				break;
			}
			case EXPR_ASSUMPTION:
			{
				exprResult += u8"Предп. " + to_string(resMap[EXPR_ASSUMPTION_KEY]);
				break;
			}
			case EXPR_MP:
			{
				exprResult += u8"M.P. " + to_string(resMap[EXPR_MP_FIRST]) + ", " + to_string(resMap[EXPR_MP_SECOND]);
				break;
			}
			case EXPR_NOT_PROVED:
			{
				exprResult += u8"Не доказано";
				break;
			}
		}
		output_f << "(" << i+1 << ") " << proofExpressions[i] << " (" << exprResult << ")" << (char) 0xA;
	}

	return 0;
}
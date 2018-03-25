//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include <vector>
#include <fstream>
#include "propositional_parser.h"
#include "propositional_proof.h"
#include <axe.h>

using namespace std;
using namespace axe;

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

	ifstream input_f(filename);

	std::string input((std::istreambuf_iterator<char>(input_f)),
					std::istreambuf_iterator<char>());

	auto variable = r_any('A', 'Z') & *(r_any('A', 'Z') | r_num());
	r_rule<str_it> unary_expr;
	auto mulOperations = r_str("&");
	auto addOperations = r_str("|");
	auto implOperations = r_str("->");

	auto multiplication = r_many(unary_expr, mulOperations);
	auto addition = r_many(multiplication, addOperations);
	auto implication = r_many(addition, implOperations);

	unary_expr = *(r_char('!')) & variable | ("(" & implication & ")");

	std::vector<string> assumptions;
	std::vector<string> proofExpressions;

	auto assumptionExpr = implication >> e_push_back(assumptions);
	auto proofExpr = implication >> e_push_back(proofExpressions);
	auto header = ~(assumptionExpr & *("," & assumptionExpr)) & r_str("|-") & implication;
	auto proof = *(proofExpr & "\n");
	auto inputGrammar = header & "\n" & proof;

	inputGrammar(input.begin(), input.end());

	std::cout << "extracted assumptions: " << std::endl;
	printVector(assumptions);
	std::cout << "extracted proof: " << std::endl;
	printVector(proofExpressions);

	PropositionalProofChecker proofChecker(assumptions, proofExpressions);

	ofstream output_f(output_filename);
	for (int i = 0; i < proofChecker.result.size(); i++)
	{
		output_f << "(" << i+1 << ") " << proofExpressions[i] << " (" << proofChecker.result[i] << ")" << std::endl;
	}

	return 0;
}
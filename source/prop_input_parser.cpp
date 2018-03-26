//
// Created by Owntage on 3/25/2018.
//

#include "prop_input_parser.h"
#include <axe.h>
#include <sstream>
#include <iostream>

typedef std::string::iterator str_it;

using namespace axe;
using namespace std;

void parsePropositionalInput(
		std::string& input,
		std::vector<std::string>& assumptions,
		std::vector<std::string>& proofExpressions,
		std::string& targetExpr)
{
	auto variable = r_any('A', 'Z') & *(r_any('A', 'Z') | r_num());
	r_rule<str_it> unary_expr;
	auto mulOperations = r_str("&");
	auto addOperations = r_str("|");
	auto implOperations = r_str("->");

	auto multiplication = r_many(unary_expr, mulOperations);
	auto addition = r_many(multiplication, addOperations);
	auto implication = r_many(addition, implOperations);

	unary_expr = *(r_char('!')) & variable | ("(" & implication & ")");

	auto assumptionExpr = implication >> e_push_back(assumptions);

	auto header = ~(assumptionExpr & *("," & assumptionExpr)) & r_str("|-") & (implication >> targetExpr);

	str_it inputIterator = header(input.begin(), input.end()).position + 1;
	input = input.substr(inputIterator - input.begin());
	std::stringstream inputStream(input);
	while (!inputStream.eof())
	{
		std::string line;
		std::getline(inputStream, line);
		if (implication(line.begin(), line.end()).matched)
		{
			proofExpressions.push_back(line);
		}
	}

}



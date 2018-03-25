//
// Created by Owntage on 3/25/2018.
//

#ifndef MATH_LOGIC_HW_PROP_INPUT_PARSER_H
#define MATH_LOGIC_HW_PROP_INPUT_PARSER_H

#include <vector>
#include <string>

void parsePropositionalInput(
		std::string& input,
		std::vector<std::string>& assumptions,
		std::vector<std::string>& proofExpressions,
		std::string& targetExpr);

#endif //MATH_LOGIC_HW_PROP_INPUT_PARSER_H

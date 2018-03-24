//
// Created by Owntage on 3/24/2018.
//

#ifndef MATH_LOGIC_HW_PROPOSITIONAL_PROOF_H
#define MATH_LOGIC_HW_PROPOSITIONAL_PROOF_H

#include "propositional_parser.h"
#include <vector>
#include <string>

int getAxiom(ExprTree* expr);

struct PropositionalProofChecker
{
	PropositionalProofChecker(std::vector<std::string> assumptions, std::vector<std::string> expressions);

	std::vector<std::string> result;
};

#endif //MATH_LOGIC_HW_PROPOSITIONAL_PROOF_H

//
// Created by Owntage on 3/24/2018.
//

#ifndef MATH_LOGIC_HW_PROPOSITIONAL_PROOF_H
#define MATH_LOGIC_HW_PROPOSITIONAL_PROOF_H

#include "propositional_parser.h"
#include <vector>
#include <string>
#include <map>

int getAxiom(ExprTree* expr);

#define EXPR_NOT_PROVED 0
#define EXPR_AXIOM 1
#define EXPR_ASSUMPTION 2
#define EXPR_MP 3

#define EXPR_TYPE_KEY 0
#define EXPR_AXIOM_KEY 1
#define EXPR_ASSUMPTION_KEY 2
#define EXPR_MP_FIRST 3
#define EXPR_MP_SECOND 4

struct PropositionalProofChecker
{
	PropositionalProofChecker(std::vector<std::string> assumptions, std::vector<std::string> expressions);

	std::vector<std::map<int, int> > result;
};

#endif //MATH_LOGIC_HW_PROPOSITIONAL_PROOF_H

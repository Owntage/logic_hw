//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include <vector>
#include "propositional_parser.h"

using namespace std;

vector<ExprTree*> assumptions;

bool checkAxiom1(ExprTree* expr)
{
	if (expr->value != "->") return false;
	if (expr->left == nullptr || expr->right == nullptr) return false;
	ExprTree* rightExpr = expr->right;
	if (rightExpr->value != "->") return false;
	if (rightExpr->left == nullptr || rightExpr->right == nullptr) return false;
	return *expr->left == *rightExpr->right;
}

bool checkAxiom2(ExprTree* expr)
{
	
}

int main()
{
	if (checkAxiom1(PropositionalParser::parse("(A->B)->(B->(A->B))")))
	{
		std::cout << "axiom1 true" << std::endl;
	}
	else
	{
		std::cout << "axiom1 false" << std::endl;
	}
	return 0;
}
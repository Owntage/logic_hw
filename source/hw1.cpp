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
	ExprTree* rightExpr = expr->right;
	if (rightExpr->value != "->") return false;
	return *expr->left == *rightExpr->right;
}

bool notImpl(ExprTree* expr)
{
	return expr->value != "->";
}

bool notEq(ExprTree* first, ExprTree* second)
{
	return !(*first == *second);
}

bool checkAxiom2(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* impl2 = impl1->left;
	if (notImpl(impl2)) return false;
	ExprTree* impl3 = impl1->right;
	if (notImpl(impl3)) return false;
	ExprTree* impl4 = impl3->left;
	if (notImpl(impl4)) return false;
	ExprTree* impl5 = impl3->right;
	if (notImpl(impl5)) return false;
	ExprTree* impl6 = impl4->right;
	if (notImpl(impl6)) return false;

	ExprTree* expr1 = impl2->left;
	ExprTree* expr2 = impl2->right;
	ExprTree* expr3 = impl6->right;

	if (notEq(impl4->left, expr1)) return false;
	if (notEq(impl6->left, expr2)) return false;
	if (notEq(impl5->left, expr1)) return false;
	return !notEq(impl5->right, expr3);
}

int main()
{
	std::cout << *PropositionalParser::parse("(A->B)->(A->B->C)->(A->C)") << std::endl;
	if (checkAxiom2(PropositionalParser::parse("(A->B)->(A->B->C)->(A->C)")))
	{
		std::cout << "axiom1 true" << std::endl;
	}
	else
	{
		std::cout << "axiom1 false" << std::endl;
	}
	return 0;
}
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

bool notAnd(ExprTree* expr)
{
	return expr->value != "&";
}

bool notOr(ExprTree* expr)
{
	return expr->value != "|";
}

bool notNeg(ExprTree* expr)
{
	return expr->value != "!";
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

bool checkAxiom3(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* impl2 = impl1->right;
	if (notImpl(impl2)) return false;
	ExprTree* and3 = impl2->right;
	if (notAnd(and3)) return false;

	ExprTree* expr1 = impl1->left;
	ExprTree* expr2 = impl2->left;

	if (notEq(and3->left, expr1)) return false;
	return !notEq(and3->right, expr2);
}

bool checkAxiom4(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* and1 = impl1->left;
	if (notAnd(and1)) return false;

	ExprTree* expr1 = and1->left;
	return !notEq(impl1->right, expr1);
}

bool checkAxiom5(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* and1 = impl1->left;
	if (notAnd(and1)) return false;

	ExprTree* expr1 = and1->right;
	return !notEq(impl1->right, expr1);
}

bool checkAxiom6(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* or1 = impl1->right;
	if (notOr(or1)) return false;

	ExprTree* expr1 = impl1->left;
	return !notEq(or1->left, expr1);
}

bool checkAxiom7(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* or1 = impl1->right;
	if (notOr(or1)) return false;

	ExprTree* expr1 = impl1->left;
	return !notEq(or1->right, expr1);
}

bool checkAxiom8(ExprTree* impl1)
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
	ExprTree* or6 = impl5->left;
	if (notOr(or6)) return false;

	ExprTree* expr1 = impl2->left;
	ExprTree* expr2 = impl4->left;
	ExprTree* expr3 = impl2->right;

	if (notEq(impl4->right, expr3)) return false;
	if (notEq(impl5->right, expr3)) return false;
	if (notEq(or6->left, expr1)) return false;
	return !notEq(or6->right, expr2);
}

bool checkAxiom9(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* impl2 = impl1->left;
	if (notImpl(impl2)) return false;
	ExprTree* impl3 = impl1->right;
	if (notImpl(impl3)) return false;
	ExprTree* impl4 = impl3->left;
	if (notImpl(impl4)) return false;
	ExprTree* neg5 = impl4->right;
	if (notNeg(neg5)) return false;
	ExprTree* neg6 = impl3->right;
	if (notNeg(neg6)) return false;

	ExprTree* expr1 = impl2->left;
	ExprTree* expr2 = impl2->right;

	if (notEq(impl4->left, expr1)) return false;
	if (notEq(neg5->left, expr2)) return false;
	return !notEq(neg6->left, expr1);
}

bool checkAxiom10(ExprTree* impl1)
{
	if (notImpl(impl1)) return false;
	ExprTree* neg1 = impl1->left;
	if (notNeg(neg1)) return false;
	ExprTree* neg2 = neg1->left;
	if (notNeg(neg2)) return false;

	ExprTree* expr = neg2->left;
	return !notEq(impl1->right, expr);
}

bool checkAxioms()
{
	if (!checkAxiom1(PropositionalParser::parse("A->(B->A)"))) return false;
	if (!checkAxiom2(PropositionalParser::parse("(A->B)->(A->B->C)->(A->C)"))) return false;
	if (!checkAxiom3(PropositionalParser::parse("A->B->A&B"))) return false;
	if (!checkAxiom4(PropositionalParser::parse("A&B->A"))) return false;
	if (!checkAxiom5(PropositionalParser::parse("A&B->B"))) return false;
	if (!checkAxiom6(PropositionalParser::parse("A->A|B"))) return false;
	if (!checkAxiom7(PropositionalParser::parse("B->A|B"))) return false;
	if (!checkAxiom8(PropositionalParser::parse("(A->C)->(B->C)->(A|B->C)"))) return false;
	if (!checkAxiom9(PropositionalParser::parse("(A->B)->(A->!B)->!A"))) return false;
	return checkAxiom10(PropositionalParser::parse("!!A->A"));
}

int main()
{
	std::cout << "axiom3: " << *PropositionalParser::parse("A->B->A&B") << std::endl;
	std::cout << "axiom4: " << *PropositionalParser::parse("A&B->A") << std::endl;
	std::cout << "axiom5: " << *PropositionalParser::parse("A&B->B") << std::endl;
	std::cout << "axiom6: " << *PropositionalParser::parse("A->A|B") << std::endl;
	std::cout << "axiom8: " << *PropositionalParser::parse("(A->C)->(B->C)->(A|B->C)") << std::endl;
	std::cout << "axiom9: " << *PropositionalParser::parse("(A->B)->(A->!B)->!A") << std::endl;
	std::cout << "axiom10: " << *PropositionalParser::parse("!!A->A") << std::endl;
	if (checkAxioms())
	{
		std::cout << "all axioms are tested" << std::endl;
	}
	else
	{
		std::cout << "failed to test axioms" << std::endl;
	}
	return 0;
}
//
// Created by Owntage on 3/24/2018.
//

#include "propositional_parser.h"
#include "propositional_proof.h"
#include <iostream>
#include <unordered_map>
#include <utility>
#include <sstream>

using namespace std;

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

int getAxiom(ExprTree* expr)
{
	if (checkAxiom1(expr)) return 1;
	if (checkAxiom2(expr)) return 2;
	if (checkAxiom3(expr)) return 3;
	if (checkAxiom4(expr)) return 4;
	if (checkAxiom5(expr)) return 5;
	if (checkAxiom6(expr)) return 6;
	if (checkAxiom7(expr)) return 7;
	if (checkAxiom8(expr)) return 8;
	if (checkAxiom9(expr)) return 9;
	if (checkAxiom10(expr)) return 10;
	return -1;
}

struct Memorization
{
	Memorization(vector<ExprTree*>& exprTrees) :
			exprTrees(exprTrees)
	{}

	pair<int, int> getMP(int i)
	{
		stringstream s;
		s << *exprTrees[i];
		string exprStr = s.str();
		if (memorization.find(exprStr) != memorization.end())
		{
			return memorization[exprStr];
		}
		bool mpFound = false;
		int mpI = -1;
		int mpJ = -1;
		for (int j = i - 1; j >= 0; j--)
		{
			for (int k = i - 1; k >= 0; k--)
			{
				if (mpFound) break;
				if (exprTrees[j]->value == "->")
				{
					if (*exprTrees[j]->left == *exprTrees[k] && *exprTrees[j]->right == *exprTrees[i])
					{
						mpFound = true;
						mpI = k;
						mpJ = j;
					}
				}
			}
			if (mpFound) break;
		}
		if (mpFound)
		{
			memorization[exprStr] = make_pair(mpI, mpJ);
		}
		return make_pair(mpI, mpJ);
	};
private:
	vector<ExprTree*>& exprTrees;
	unordered_map<string, pair<int, int> > memorization;
};

PropositionalProofChecker::PropositionalProofChecker(std::vector<std::string> assumptions,
													 std::vector<std::string> expressions)
{
	std::vector<ExprTree*> assumpExpressions;
	for (int i = 0; i < assumptions.size(); i++)
	{
		assumpExpressions.push_back(PropositionalParser::parse(assumptions[i]));
	}

	std::vector<ExprTree*> exprTrees;
	for(int i = 0; i < expressions.size(); i++)
	{
		exprTrees.push_back(PropositionalParser::parse(expressions[i]));
	}

	Memorization memorization(exprTrees);

	result.resize(expressions.size());
	for (int i = 0; i < exprTrees.size(); i++)
	{
		if (i % 500 == 0)
		{
			std::cout << "progress: " << ((float) i) / ((float) exprTrees.size()) << std::endl;
		}

		int axiom = getAxiom(exprTrees[i]);
		if (axiom != -1)
		{
			//result[i] = "axiom " + std::to_string(axiom);
			result[i][EXPR_TYPE_KEY] = EXPR_AXIOM;
			result[i][EXPR_AXIOM_KEY] = axiom;
			continue;
		}

		auto mp = memorization.getMP(i);

		if (mp.first != -1)
		{
			//result[i] = "M.P. " + std::to_string(mpI + 1) + " " + std::to_string(mpJ + 1);
			result[i][EXPR_TYPE_KEY] = EXPR_MP;
			result[i][EXPR_MP_FIRST] = mp.first + 1;
			result[i][EXPR_MP_SECOND] = mp.second + 1;
			continue;
		}

		bool isAssumption = false;
		for (int j = 0; j < assumpExpressions.size(); j++)
		{
			if (*exprTrees[i] == *assumpExpressions[j])
			{
				//result[i] = "assumption " + std::to_string(j+1);
				result[i][EXPR_TYPE_KEY] = EXPR_ASSUMPTION;
				result[i][EXPR_ASSUMPTION_KEY] = j+1;
				isAssumption = true;
			}
		}
		if (isAssumption) continue;

		//result[i] = "not prooved";
		result[i][EXPR_TYPE_KEY] = EXPR_NOT_PROVED;
	}
}
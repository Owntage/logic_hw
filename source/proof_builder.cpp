//
// Created by Owntage on 3/27/2018.
//

#include "proof_builder.h"
#include "deduction.h"

using namespace std;

static std::string getInfixExpr(ExprTree *tree)
{
	if (tree->value == "!")
	{
		return "!(" + getInfixExpr(tree->left) + ")";
	}
	if (tree->left != nullptr && tree->right != nullptr)
	{
		return "(" + getInfixExpr(tree->left) + ")" + tree->value + "(" + getInfixExpr(tree->right) + ")";
	}
	return tree->value;
}

static std::string neg(const std::string &expr)
{
	return "!(" + expr + ")";
}

static std::string impl(const std::string &expr1, const std::string &expr2)
{
	return "(" + expr1 + ")->(" + expr2 + ")";
}

std::vector<std::string> proveImplication(ExprTree *target, const std::map<std::string, bool> &values)
{
	//todo: finish
}

void printAToA(vector<string> &list, string target)
{
	list.push_back(impl(
			target,
			impl(
					target,
					target
			)
	));
	list.push_back(impl(
			target,
			impl(
					impl(target, target),
					target
			)
	));
	list.push_back(impl(
			impl(
					target,
					impl(target, target)
			),
			impl(
					impl(
							target,
							impl(
									impl(target, target),
									target
							)
					),
					impl(target, target)
			)
	));
	list.push_back(impl(
			impl(
					target,
					impl(
							impl(target, target),
							target
					)
			),
			impl(target, target)
	));
	list.push_back(impl(target, target));
}

template<typename T>
void operator<<(vector<T> &v1, const vector<T> &v2)
{
	for (int i = 0; i < v2.size(); i++)
	{
		v1.push_back(v2);
	}
}

std::vector<std::string> proveNotFirstNotSecondImplication(string first, string second)
{
	//todo: finish
}

std::vector<std::string> proveDisjunction(ExprTree *target, const std::map<std::string, bool> &values)
{
	ExprTree *firstExpr = target->left;
	ExprTree *secondExpr = target->right;
	string first = getInfixExpr(firstExpr);
	string second = getInfixExpr(secondExpr);
	string targetStr = getInfixExpr(target);
	string mainVariable;

	vector<string> list;

	if (!firstExpr->isTrueOn(values) && !secondExpr->isTrueOn(values) && !target->isTrueOn(values))
	{
		vector<string> sublist;
		sublist.push_back(neg(first));
		sublist.push_back(neg(second));
		sublist.push_back(getInfixExpr(target));
		printAToA(sublist, first);

		list.push_back((neg(first)));
		list.push_back((neg(second)));
		list.push_back((impl(
				impl(targetStr, first),
				impl(
						impl(targetStr, neg(first)),
						neg(targetStr)
				)
		)));
		list.push_back((impl(
				neg(first),
				impl(targetStr, neg(first))
		)));
		list.push_back((impl(targetStr, neg(first))));

		vector<string> deductionAssumptions;
		vector<string> deductionProof;

		deductionAssumptions.push_back((neg(first)));
		deductionAssumptions.push_back((neg(second)));
		deductionAssumptions.push_back(targetStr);

		deductionProof << sublist;
		deductionProof << proveNotFirstNotSecondImplication(second, first);
		deductionProof.push_back((impl(second, first)));
		deductionProof.push_back((impl(
				impl(first, first),
				impl(
						impl(second, first),
						impl(targetStr, first)
				)
		)));
		deductionProof.push_back((impl(
				impl(second, first),
				impl(targetStr, first)
		)));
		deductionProof.push_back((impl(targetStr, first)));
		deductionProof.push_back((first));

		list << buildDeductiveProof(deductionAssumptions, deductionProof);
		list.push_back(impl(targetStr, first));
		list.push_back((impl(
				impl(targetStr, neg(first)),
				neg(targetStr))));
		list.push_back((neg(targetStr)));
		return list;
	}

	if (!firstExpr->isTrueOn(values) && secondExpr->isTrueOn(values) && target->isTrueOn(values))
	{
		mainVariable = second;
	} else
	{
		mainVariable = first;
	}
	list.push_back(mainVariable);
	list.push_back(impl(mainVariable, targetStr));
	list.push_back(targetStr);
	return list;
}

std::vector<std::string> proveConjunction(ExprTree *target, const std::map<std::string, bool> &values)
{
	//todo: finish
}

std::vector<std::string> proveNegation(ExprTree *target, const std::map<std::string, bool> &values)
{
	//todo: finish
}

std::vector<std::string> prove(ExprTree *target, const std::map<std::string, bool> &values)
{
	if (target->value == "!") return proveNegation(target, values);
	if (target->value == "&") return proveConjunction(target, values);
	if (target->value == "|") return proveDisjunction(target, values);
	if (target->value == "->") return proveImplication(target, values);
	std::vector<std::string> empty;
	return empty;
}

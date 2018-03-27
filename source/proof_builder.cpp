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

vector<string> proveNotFirstNotSecondImplication(string, string);

std::vector<std::string> proveImplication(ExprTree *target, const std::map<std::string, bool> &values)
{
	ExprTree *firstExpr = target->left;
	ExprTree *secondExpr = target->right;
	string first = getInfixExpr(firstExpr);
	string second = getInfixExpr(secondExpr);
	string targetStr = getInfixExpr(target);

	if (!firstExpr->isTrueOn(values) && !secondExpr->isTrueOn(values) && target->isTrueOn(values))
	{
		return proveNotFirstNotSecondImplication(first, second);
	}

	vector<string> list;
	if (firstExpr->isTrueOn(values) && !secondExpr->isTrueOn(values) && !target->isTrueOn(values))
	{
		list.push_back((first));
		list.push_back((neg(second)));
		list.push_back((impl(
				neg(second),
				impl(targetStr, neg(second))
		)));
		list.push_back((neg(second)));
		list.push_back((impl(targetStr, neg(second))));

		vector<string> deductionAssumptions;
		vector<string> deductionProof;

		deductionAssumptions.push_back((first));
		deductionAssumptions.push_back((neg(second)));
		deductionAssumptions.push_back(targetStr);
		deductionProof.push_back((first));
		deductionProof.push_back(targetStr);
		deductionProof.push_back((second));
		list << (buildDeductiveProof(deductionAssumptions, deductionProof));

		list.push_back((impl(
				impl(targetStr, second),
				impl(
						impl(targetStr, neg(second)),
						neg(targetStr)
				)
		)));
		list.push_back((impl(
				impl(targetStr, neg(second)),
				neg(targetStr)
		)));
		list.push_back((neg(targetStr)));
		return list;
	}

	list.push_back((second));
	list.push_back((impl(second, targetStr)));
	list.push_back(targetStr);
	return list;
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
static void operator<<(vector<T> &v1, const vector<T> &v2)
{
	for (int i = 0; i < v2.size(); i++)
	{
		v1.push_back(v2);
	}
}

std::vector<std::string> proveNotFirstNotSecondImplication(string first, string second)
{
	vector<string> deductionAssumptions;
	vector<string> deductionProof;

	deductionAssumptions.push_back((neg(first)));
	deductionAssumptions.push_back((neg(second)));
	deductionAssumptions.push_back((first));

	deductionProof.push_back((impl(
			impl(neg(second), first),
			impl(
					impl(neg(second), neg(first)),
					neg(neg(second))
			)
	)));
	deductionProof.push_back((first));
	deductionProof.push_back((impl(
			first,
			impl(neg(second), first))));
	deductionProof.push_back((impl(neg(second), first)));
	deductionProof.push_back((impl(
			impl(
					neg(second),
					neg(first)
			),
			neg(neg(second))
	)));
	deductionProof.push_back((neg(first)));
	deductionProof.push_back((impl(
			neg(first),
			impl(neg(second), neg(first))
	)));
	deductionProof.push_back((impl(neg(second), neg(first))));
	deductionProof.push_back((neg(neg(second))));
	deductionProof.push_back((impl(neg(neg(second)), second)));
	deductionProof.push_back((second));

	return buildDeductiveProof(deductionAssumptions, deductionProof);
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
	ExprTree *firstExpr = target->left;
	ExprTree *secondExpr = target->right;
	string first = getInfixExpr(firstExpr);
	string second = getInfixExpr(secondExpr);
	string targetStr = getInfixExpr(target);
	string mainVariable;

	vector<string> list;
	if (firstExpr->isTrueOn(values) && secondExpr->isTrueOn(values) && target->isTrueOn(values))
	{
		list.push_back((first));
		list.push_back((second));
		list.push_back((impl(
				first,
				impl(second, targetStr)
		)));
		list.push_back((impl(second, targetStr)));
		list.push_back(targetStr);
		return list;
	}
	if (firstExpr->isTrueOn(values) && !secondExpr->isTrueOn(values) && !target->isTrueOn(values))
		mainVariable = second;
	else
		mainVariable = first;
	list.push_back((neg(mainVariable)));
	list.push_back((impl(
			impl(targetStr, mainVariable),
			impl(
					impl(targetStr, neg(mainVariable)),
					neg(targetStr))
	)));
	list.push_back((impl(targetStr, mainVariable)));
	list.push_back((impl(
			impl(targetStr, neg(mainVariable)),
			neg(targetStr)
	)));
	list.push_back((impl(
			neg(mainVariable),
			impl(targetStr, neg(mainVariable))
	)));
	list.push_back((impl(targetStr, neg(mainVariable))));
	list.push_back((neg(targetStr)));
	return list;
}

std::vector<std::string> proveNegation(ExprTree *target, const std::map<std::string, bool> &values)
{
	ExprTree *firstExpr = target->left;
	string first = getInfixExpr(firstExpr);
	string targetStr = getInfixExpr(target);

	vector<string> list;
	if (!firstExpr->isTrueOn(values) && target->isTrueOn(values))
		list.push_back(targetStr);
	else
	{
		list.push_back((first));
		list.push_back((impl(first, impl(neg(first), first))));
		list.push_back((impl(neg(first), first)));
		list.push_back((impl(
				neg(first),
				impl(neg(first), neg(first)))));
		list.push_back((impl(
				impl(neg(first), impl(neg(first), neg(first))),
				impl(
						impl(
								neg(first),
								impl(
										impl(neg(first), neg(first)),
										neg(first)
								)
						),
						impl(neg(first), neg(first))
				)
		)));
		list.push_back((impl(
				impl(
						neg(first),
						impl(
								impl(neg(first), neg(first)),
								neg(first)
						)
				),
				impl(neg(first), neg(first))
		)));
		list.push_back((impl(
				neg(first),
				impl(
						impl(neg(first), neg(first)),
						neg(first)
				)
		)));
		list.push_back((impl(neg(first), neg(first))));
		list.push_back((impl(
				impl(neg(first), first),
				impl(
						impl(neg(first), neg(first)),
						neg(neg(first))
				)
		)));
		list.push_back((impl(
				impl(neg(first), neg(first)),
				neg(neg(first))
		)));
		list.push_back((neg(neg(first))));
	}
	return list;
}

namespace ProofBuilder
{
	std::vector<std::string> prove(ExprTree *target, const std::map<std::string, bool> &values)
	{
		if (target->value == "!") return proveNegation(target, values);
		if (target->value == "&") return proveConjunction(target, values);
		if (target->value == "|") return proveDisjunction(target, values);
		if (target->value == "->") return proveImplication(target, values);
		std::vector<std::string> empty;
		return empty;
	}
}

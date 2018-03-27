//
// Created by Owntage on 3/27/2018.
//

#include "proof_builder.h"

static std::string getInfixExpr(ExprTree* tree)
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

static std::string neg(const std::string& expr)
{
	return "!(" + expr + ")";
}

static std::string impl(const std::string& expr1, const std::string& expr2)
{
	return "(" + expr1 + ")->(" + expr2 + ")";
}

std::vector<std::string> proveImplication(ExprTree* target, const std::map<std::string, bool>& values)
{
	//todo: finish
}

std::vector<std::string> proveDisjunction(ExprTree* target, const std::map<std::string, bool>& values)
{
	//todo: finish
}

std::vector<std::string> proveConjunction(ExprTree* target, const std::map<std::string, bool>& values)
{
	//todo: finish
}

std::vector<std::string> proveNegation(ExprTree* target, const std::map<std::string, bool>& values)
{
	//todo: finish
}

std::vector<std::string> prove(ExprTree* target, const std::map<std::string, bool>& values)
{
	if (target->value == "!") return proveNegation(target, values);
	if (target->value == "&") return proveConjunction(target, values);
	if (target->value == "|") return proveDisjunction(target, values);
	if (target->value == "->") return proveImplication(target, values);
	std::vector<std::string> empty;
	return empty;
}

//
// Created by Owntage on 3/23/2018.
//

#include<axe.h>
#include<stack>
#include "propositional_parser.h"

using namespace axe;

typedef std::string::iterator str_it;

bool ExprTree::operator==(const ExprTree& other)
{
	if (this->value != other.value) return false;
	if ((this->left == nullptr) != (other.left == nullptr)) return false;
	if ((this->right == nullptr) != (other.right == nullptr)) return false;

	bool result = true;
	if (this->left != nullptr)
	{
		result = *this->left == *other.left;
	}
	if (this->right != nullptr)
	{
		result = result && *this->right == *other.right;
	}
	return result;
}

template<typename BaseRule, typename OpRule>
ExprTree* generateTree(std::string input, const BaseRule& baseRule, std::vector<OpRule> operationRules, int offset = 0);

template<typename BaseRule, typename OpRule>
ExprTree* generateBaseLevelTree(std::string input, const BaseRule& baseRule, std::vector<OpRule> operationRules)
{
	std::vector<std::string> operands;
	std::vector<std::string> operations;
	auto operandRule = baseRule >> e_push_back(operands);
	auto operationExtractorRule = operationRules[0] >> e_push_back(operations);
	auto exprRule = r_many(operandRule, operationExtractorRule);
	exprRule(input.begin(), input.end());
	std::vector<ExprTree*> resultTrees;
	for(int i = 0; i < operands.size(); i++)
	{
		if (operands[i][0] == '(')
		{
			resultTrees.push_back(generateTree(input.substr(1, input.size() - 1), baseRule, operationRules));
		}
		else
		{
			resultTrees.push_back(new ExprTree(operands[i]));
		}
		if (i != operands.size() - 1)
		{
			resultTrees.push_back(new ExprTree(operations[i]));
		}
	}
	if (resultTrees.size() > 1)
	{
		resultTrees[1]->left = resultTrees[0];
		resultTrees[1]->right = resultTrees[2];
	}
	for (int i = 3; i < operands.size(); i+= 2)
	{
		resultTrees[i]->left = resultTrees[i-2];
		resultTrees[i]->right = resultTrees[i+1];
	}
	if (resultTrees.size() > 1)
	{
		return resultTrees[resultTrees.size() - 2];
	}
	return resultTrees[0];
};

template<typename BaseRule, typename OpRule>
ExprTree* generateTree(std::string input, const BaseRule& baseRule, std::vector<OpRule> operationRules, int offset = 0)
{
	if (operationRules.size() - offset == 0)
	{
		return new ExprTree(input);
	}
	if (operationRules.size() - offset == 1)
	{
		return generateBaseLevelTree(input, baseRule, operationRules);
	}


	r_rule<str_it> operand_rule;
	operand_rule = r_many(baseRule, operationRules[0]);

	for (int i = 1; i < operationRules.size() - 1 - offset; i++)
	{
		operand_rule = r_many(operand_rule, operationRules[i]);
	}
	std::vector<ExprTree*> operandTrees;


	auto operand_tree_rule = *operand_rule >> e_ref([&operandTrees, &operationRules, &baseRule, offset](str_it begin, str_it end)
													{
														std::string matched_str(begin, end);
														operandTrees.push_back(generateTree(matched_str, baseRule, operationRules, offset + 1));
													});

	std::vector<std::string> operators;

	auto op_with_extract = operationRules[operationRules.size() - 1 - offset] >> e_push_back(operators);
	auto expr_rule = r_many(operand_tree_rule, op_with_extract);

	expr_rule(input.begin(), input.end());

	std::stack<ExprTree*> trees;
	for (int i = 0; i < operandTrees.size(); i++)
	{
		trees.push(operandTrees[operandTrees.size() - 1 - i]);
	}

	int operatorIndex = 0;
	while (trees.size() > 1)
	{
		ExprTree* first = trees.top();
		trees.pop();
		ExprTree* second = trees.top();
		trees.pop();
		ExprTree* newTree = new ExprTree(operators[operatorIndex++], first, second);
		trees.push(newTree);
	}
	return trees.top();
}

namespace PropositionalParser
{
	ExprTree* parse(const std::string& expr)
	{
		static auto variable = r_any("abc");
		static auto constant = r_numstr();
		static r_rule<str_it> unary_expr;
		static auto mulOperations = r_any("*");
		static auto addOperations = r_any("+");

		static auto multiplication = r_many(unary_expr, mulOperations);
		static auto addition = r_many(multiplication, addOperations);
		unary_expr = variable | constant | ("(" & addition & ")");

		return generateTree<decltype(unary_expr), decltype(mulOperations)>(expr, unary_expr, {mulOperations, addOperations}, 0);
	}
}

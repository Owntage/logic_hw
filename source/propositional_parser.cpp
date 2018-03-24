//
// Created by Owntage on 3/23/2018.
//

#include<axe.h>
#include<stack>
#include <c++/iostream>
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

auto variable = r_any('A', 'Z') & *(r_any('A', 'Z') | r_num());
r_rule<str_it> unary_expr;
auto mulOperations = r_str("&");
auto addOperations = r_str("|");
auto implOperations = r_str("->");

auto multiplication = r_many(unary_expr, mulOperations);
auto addition = r_many(multiplication, addOperations);
auto implication = r_many(addition, implOperations);

enum class Associativity
{
	Left,
	Right
};

auto operationRules = std::make_tuple(mulOperations, addOperations, implOperations);
auto associativity = std::make_tuple(Associativity::Right, Associativity::Left, Associativity::Left);
auto expressionRules = std::make_tuple(addition, multiplication);

void initUnaryExpr()
{
	unary_expr = ~(r_char('!')) & variable | ("(" & implication & ")");
}


template<int offset = 0>
ExprTree* generateTree(std::string input);


ExprTree* generateBaseLevelTree(std::string input)
{
	std::vector<std::string> operands;
	std::vector<std::string> operations;
	auto operandRule = unary_expr >> e_push_back(operands);
	auto operationExtractorRule = std::get<0>(operationRules) >> e_push_back(operations);
	auto exprRule = r_many(operandRule, operationExtractorRule);
	exprRule(input.begin(), input.end());
	std::vector<ExprTree*> resultTrees;
	for(int i = 0; i < operands.size(); i++)
	{
		if (operands[i][0] == '(')
		{
			resultTrees.push_back(generateTree<0>(input.substr(1, input.size() - 1)));
		}
		else
		{
			if (operands[i][0] == '!')
			{
				resultTrees.push_back(new ExprTree("!", generateTree<0>(input.substr(1)), nullptr));
			}
			else
			{
				resultTrees.push_back(new ExprTree(operands[i]));
			}
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

template<int offset = 0>
ExprTree* generateTree(std::string input)
{
	if (std::tuple_size<decltype(operationRules)>::value - offset == 0)
	{
		return new ExprTree(input);
	}
	if (std::tuple_size<decltype(operationRules)>::value - offset == 1)
	{
		return generateBaseLevelTree(input);
	}

	auto operand_rule = std::get<offset < std::tuple_size<decltype(expressionRules)>::value ? offset : 0>(expressionRules);
	std::vector<ExprTree*> operandTrees;


	auto operand_tree_rule = *operand_rule >> e_ref([&operandTrees](str_it begin, str_it end)
	{
		std::string matched_str(begin, end);
		operandTrees.push_back(generateTree<offset + 1 < std::tuple_size<decltype(operationRules)>::value ? offset + 1 : 0>(matched_str));
	});

	std::vector<std::string> operators;

	auto op_with_extract = std::get<offset >= 0 ? std::tuple_size<decltype(operationRules)>::value - 1 - offset : 0>(operationRules) >> e_push_back(operators);
	auto expr_rule = r_many(operand_tree_rule, op_with_extract);

	expr_rule(input.begin(), input.end());

	std::stack<ExprTree*> trees;
	for (int i = 0; i < operandTrees.size(); i++)
	{
		if (std::get<offset>(associativity) == Associativity::Left)
		{
			trees.push(operandTrees[operandTrees.size() - 1 - i]);
		}
		else
		{
			trees.push(operandTrees[i]);
		}

	}

	int operatorIndex = 0;
	while (trees.size() > 1)
	{
		ExprTree* first = trees.top();
		trees.pop();
		ExprTree* second = trees.top();
		trees.pop();
		if (std::get<offset>(associativity) == Associativity::Right)
		{
			std::swap(first, second);
		}
		ExprTree* newTree = new ExprTree(operators[operatorIndex++], first, second);
		trees.push(newTree);
	}
	return trees.top();
}


namespace PropositionalParser
{
	ExprTree* parse(const std::string& expr)
	{
		initUnaryExpr();

		return generateTree(expr);
	}
}

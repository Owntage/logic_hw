//
// Created by Owntage on 3/23/2018.
//

#ifndef MATH_LOGIC_HW_PROPOSITIONAL_PARSER_H
#define MATH_LOGIC_HW_PROPOSITIONAL_PARSER_H

#include <string>

#define UNARY_EXPR_DEF variable | (*r_str("!") & "(" & implication & ")") | ("(" & implication & ")") | (*r_str("!") & variable)

struct ExprTree
{
	std::string value;
	ExprTree* left;
	ExprTree* right;

	ExprTree(const std::string& value) :
			value(value),
			left(nullptr),
			right(nullptr)
	{}

	ExprTree(const std::string& value, ExprTree* left, ExprTree* right) :
			value(value),
			left(left),
			right(right)
	{}

	bool operator==(const ExprTree& other);

	template<typename Stream>
	friend Stream& operator<<(Stream& s, ExprTree& tree)
	{
		s << tree.value;
		if (tree.left != nullptr && tree.right != nullptr)
		{
			s << "(" << *tree.left << ", " << *tree.right << ")";
		}
		else if (tree.left != nullptr)
		{
			s << "(" << *tree.left << ")";
		}
		return s;
	}
};

namespace PropositionalParser
{
	ExprTree* parse(const std::string& expr);
}

#endif //MATH_LOGIC_HW_PROPOSITIONAL_PARSER_H

//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include "propositional_parser.h"

bool compareExpressionTrees()
{
	std::string expr("a|b&c");
	std::string anotherExpr("a|a|a");
	ExprTree* exprTree1 = PropositionalParser::parse(expr);
	ExprTree* exprTree2 = PropositionalParser::parse(expr);
	ExprTree* exprTree3 = PropositionalParser::parse(anotherExpr);
	if (!(*exprTree1 == *exprTree2)) return false;
	if (*exprTree1 == *exprTree3) return false;

	std::cout << "ExprTree tests passed" << std::endl;
	return true;
}

bool checkGeneratedTree()
{
	ExprTree* tree1 = new ExprTree("|", new ExprTree("&", new ExprTree("a"), new ExprTree("b")), new ExprTree("c"));
	ExprTree* tree2 = PropositionalParser::parse("a&b|c");
	if (!(*tree1 == *tree2))
	{
		std::cout << "extected: " << *tree1 << std::endl;
		std::cout << "found: " << *tree2 << std::endl;
		return false;
	}

	std::cout << "tree generation test finished" << std::endl;
	return true;
}

int main()
{
	if (!compareExpressionTrees()) return 1;
	if (!checkGeneratedTree()) return 1;
	std::cout << *PropositionalParser::parse("a->b->c") << std::endl;
	std::cout << *PropositionalParser::parse("a|b|c") << std::endl;
	return 0;
}
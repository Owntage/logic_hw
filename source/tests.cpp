//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include "propositional_parser.h"

bool compareExpressionTrees()
{
	std::string expr("A|B&C");
	std::string anotherExpr("A|A|A");
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
	ExprTree* tree1 = new ExprTree("|", new ExprTree("&", new ExprTree("A"), new ExprTree("B")), new ExprTree("C"));
	ExprTree* tree2 = PropositionalParser::parse("A&B|C");
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
	std::cout << *PropositionalParser::parse("A->B->C") << std::endl;
	std::cout << *PropositionalParser::parse("!A|!B|!C") << std::endl;
	return 0;
}
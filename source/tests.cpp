//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include "propositional_parser.h"
#include "propositional_proof.h"

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

bool isTrueOnTest(std::string expr, std::map<std::string, bool> values, bool expected)
{
	ExprTree* exprTree = PropositionalParser::parse(expr);
	return exprTree->isTrueOn(values) == expected;
}

bool isTrueOnTests()
{
	bool res = true;
	res |= isTrueOnTest("A&!B", {{"A", true}, {"B", false}}, true);
	res |= isTrueOnTest("A->(B->A)", {{"A", true}, {"B", false}}, true);
	res |= isTrueOnTest("A->(B->A)", {{"A", false}, {"B", false}}, true);
	res |= isTrueOnTest("A|!B", {{"A", false}, {"B", false}}, true);
	res |= isTrueOnTest("A->B", {{"A", true}, {"B", false}}, false);
	res |= isTrueOnTest("(!A->!B)->(!A->!!B)->(!!A)", {{"A", false}, {"B", false}}, true);
	return res;
}


int main()
{
	if (!compareExpressionTrees()) return 1;
	if (!checkGeneratedTree()) return 1;
	if (!isTrueOnTests()) return 1;

	ExprTree* tree = PropositionalParser::parse("(!A->!B)->(!A->!!B)->(!!A)");
	std::cout << "parsed tree: " << *tree << std::endl;
	std::cout << "axiom: " << getAxiom(tree) << std::endl;
	return 0;
}
//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include "propositional_parser.h"

using namespace std;

int main()
{
	ExprTree* result = PropositionalParser::parse("(a)+(b)");
	cout << "parsed expression: " << *result << ". " << endl;
	return 0;
}
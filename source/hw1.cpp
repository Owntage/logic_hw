//
// Created by Owntage on 3/22/2018.
//

#include <iostream>
#include <vector>
#include "propositional_parser.h"

using namespace std;


int main()
{
	std::cout << "axiom3: " << *PropositionalParser::parse("A->B->A&B") << std::endl;
	std::cout << "axiom4: " << *PropositionalParser::parse("A&B->A") << std::endl;
	std::cout << "axiom5: " << *PropositionalParser::parse("A&B->B") << std::endl;
	std::cout << "axiom6: " << *PropositionalParser::parse("A->A|B") << std::endl;
	std::cout << "axiom8: " << *PropositionalParser::parse("(A->C)->(B->C)->(A|B->C)") << std::endl;
	std::cout << "axiom9: " << *PropositionalParser::parse("(A->B)->(A->!B)->!A") << std::endl;
	std::cout << "axiom10: " << *PropositionalParser::parse("!!A->A") << std::endl;
	return 0;
}
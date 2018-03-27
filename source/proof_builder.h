//
// Created by Owntage on 3/27/2018.
//

#ifndef MATH_LOGIC_HW_PROOF_BUILDER_H
#define MATH_LOGIC_HW_PROOF_BUILDER_H

#include <vector>
#include <string>
#include <map>
#include "propositional_parser.h"

namespace ProofBuilder
{
	std::vector<std::string> prove(ExprTree* target, const std::map<std::string, bool>& values);
}

#endif //MATH_LOGIC_HW_PROOF_BUILDER_H

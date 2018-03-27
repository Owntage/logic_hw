//
// Created by Owntage on 3/27/2018.
//

#include <string>
#include <vector>
#include "proof_builder.h"
#include "deduction.h"

using namespace std;

static std::string neg(const std::string &expr)
{
	return "!(" + expr + ")";
}

static std::string impl(const std::string &expr1, const std::string &expr2)
{
	return "(" + expr1 + ")->(" + expr2 + ")";
}

static std::string conj(const std::string &expr1, const std::string &expr2)
{
	return "(" + expr1 + ")&(" + expr2 + ")";
}

static std::string disj(const std::string &expr1, const std::string &expr2)
{
	return "(" + expr1 + ")|(" + expr2 + ")";
}

string statement;

vector<string> counterPosition(string a, string b)
{
	vector<string> impl;
	vector<string> proof;
	impl.push_back((impl(a, b)));
	vector<string> copy(impl);
	copy.push_back((neg(b)));

	proof.push_back((impl(
			impl(a, b),
			impl(impl(a, neg(b)), neg(a))
	)));
	proof.push_back((impl(a, b)));
	proof.push_back((impl(
			impl(a, neg(b)),
			neg(a)
	)));
	proof.push_back((impl(
			neg(b),
			impl(a, neg(b)))
	));
	proof.push_back((neg(b)));
	proof.push_back((impl(a, neg(b))));
	proof.push_back((neg(a)));

	return buildDeductiveProof(impl, buildDeductiveProof(copy, proof));
}

vector<string> aEnd(string v)
{
	vector<string> list;
	list.push_back((impl(
			impl(v, statement),
			impl(
					impl(neg(v), statement),
					impl(
							disj(v, neg(v)),
							statement
					)
			)
	)));
	list.push_back((impl(
			impl(neg(v), statement),
			impl(
					disj(v, neg(v)),
					statement
			)
	)));
	list.push_back((
			impl(
					disj(v, neg(v)),
					statement
			)
	));
	list.push_back(statement);
	return list;
}



int main()
{
	return 0;
}
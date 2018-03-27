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

template<typename T>
static void operator<<(vector<T> &v1, const vector<T> &v2)
{
	for (int i = 0; i < v2.size(); i++)
	{
		v1.push_back(v2);
	}
}

string statement;

vector<string> counterPosition(string, string);

vector<string> aOrNotA(string a)
{
	vector<string> proof;

	proof.push_back((impl(
			a,
			disj(a, neg(a)))));
	proof << (counterPosition((a), (disj(a, neg(a)))));
	proof.push_back((impl(
			neg(disj(a, neg(a))),
			neg(a))
	));
	proof.push_back((impl(
			neg(a),
			disj(a, neg(a)))
	));
	proof << (counterPosition((neg(a)), (disj(a, neg(a)))));
	proof.push_back((impl(
			neg(disj(a, neg(a))),
			neg(neg(a)))
	));
	proof.push_back((impl(
			impl(
					neg(disj(a, neg(a))),
					neg(a)),
			impl(
					impl(
							neg(disj(a, neg(a))),
							neg(neg(a))),
					neg(neg(disj(a, neg(a))))
			)
	)));
	proof.push_back((impl(
			impl(
					neg(disj(a, neg(a))),
					neg(neg(a))),
			neg(neg(disj(a, neg(a))))
	)));
	proof.push_back((neg(neg(disj(a, neg(a))))));
	proof.push_back((impl(
			neg(neg(disj(a, neg(a)))),
			disj(a, neg(a))
	)));
	proof.push_back((disj(a, neg(a))));
	return proof;
}

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
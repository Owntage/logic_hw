//
// Created by Owntage on 3/27/2018.
//

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include "proof_builder.h"
#include "deduction.h"

using namespace std;

static std::string neg(const std::string &expr)
{
	return "!(" + expr + ")";
}

static std::string impl(std::string expr1, std::string expr2)
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
		v1.push_back(v2[i]);
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
	vector<string> implVec;
	vector<string> proof;
	implVec.push_back(impl(a, b));
	vector<string> copy(implVec);
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

	return buildDeductiveProof(implVec, buildDeductiveProof(copy, proof));
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

void extractVariables(ExprTree* expr, set<string>& variables)
{
	if (expr->value == "&" || expr->value == "|" || expr->value == "->")
	{
		extractVariables(expr->left, variables);
		extractVariables(expr->right, variables);
		return;
	}
	if (expr->value == "!")
	{
		extractVariables(expr->left, variables);
		return;
	}
	variables.insert(expr->value);
}

typedef map<string, bool> val_map;

vector<val_map> generateValues(vector<string>& variables, int offset = 0)
{
	vector<val_map> result;
	if (offset == variables.size() - 1)
	{
		val_map m;
		m[variables.back()] = true;
		result.push_back(m);
		m[variables.back()] = false;
		result.push_back(m);
		return result;
	}
	result << generateValues(variables, offset + 1);
	result.back()[variables[offset]] = true;
	result << generateValues(variables, offset + 1);
	result.back()[variables[offset]] = false;
	return result;
};

int main()
{
	string input_filename;
	string output_filename;
	cout << "enter input filename: " << endl;
	cin >> input_filename;
	cout << "enter output filename: " << endl;
	cin >> output_filename;

	ifstream input_f(input_filename);
	ofstream output_f(output_filename);

	getline(input_f, statement);

	ExprTree* inputTree = PropositionalParser::parse(statement);
	set<string> variables;
	extractVariables(inputTree, variables);
	vector<string> variablesVec(variables.begin(), variables.end());
	vector<val_map> values = generateValues(variablesVec);

	for (int i = 0; i < values.size(); i++)
	{
		if (!inputTree->isTrueOn(values[i]))
		{
			output_f << u8"Высказывание ложно при ";
			for (auto var_it = values[i].begin(); var_it != values[i].end(); var_it++)
			{
				if (var_it != values[i].begin())
				{
					output_f << ", ";
				}
				output_f << var_it->first << "=";
				if (var_it->second)
				{
					output_f << u8"И";
				}
				else
				{
					output_f << u8"Л";
				}
			}
		}
	}


	return 0;
}
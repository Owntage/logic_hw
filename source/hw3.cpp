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

static std::string getInfixExpr(ExprTree *tree)
{
	if (tree->value == "!")
	{
		return "!(" + getInfixExpr(tree->left) + ")";
	}
	if (tree->left != nullptr && tree->right != nullptr)
	{
		return "(" + getInfixExpr(tree->left) + ")" + tree->value + "(" + getInfixExpr(tree->right) + ")";
	}
	return tree->value;
}

template<typename T>
static void operator<<(vector<T> &v1, const vector<T> &v2)
{
	for (int i = 0; i < v2.size(); i++)
	{
		v1.push_back(v2[i]);
	}
}

template<>
void operator<< <std::string>(vector<std::string>& v1, const vector<std::string>& v2)
{
	for (int i = 0; i < v2.size(); i++)
	{
		v1.push_back(v2[i]);
		//if (v1.size() == 67)
		//{
		//	std::cout << "here it is!: " << v1.back() << std::endl;
		//}
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

void getOperatorExpressions(ExprTree* expr, vector<ExprTree*>& output)
{
	if (expr->value == "&" || expr->value == "|" | expr->value == "->")
	{
		getOperatorExpressions(expr->left, output);
		getOperatorExpressions(expr->right, output);
		output.push_back(expr);
		return;
	}
	if (expr->value == "!")
	{
		getOperatorExpressions(expr->left, output);
		output.push_back(expr);
	}
}

vector<string> getBaseProof(vector<string> targets, val_map values)
{
	set<string> proved;
	vector<string> proof;
	for (int i = 0; i < targets.size(); i++)
	{
		if (proved.count(targets[i]) == 0)
		{
			proof << ProofBuilder::prove(PropositionalParser::parse(targets[i]), values);
			proved.insert(targets[i]);
		}
	}
	return proof;
}

vector<string> getBaseProof(vector<ExprTree*> targets, val_map values)
{
	vector<string> strings;
	for (int i = 0; i < targets.size(); i++)
	{
		strings.push_back(getInfixExpr(targets[i]));
	}
	return getBaseProof(strings, values);
}

val_map getValues(vector<string> assumptions)
{
	val_map result;
	for (int i = 0; i < assumptions.size(); i++)
	{
		if (assumptions[i][0] == '!')
		{
			result[assumptions[i].substr(1)] = false;
		}
		else
		{
			result[assumptions[i]] = true;
		}
	}
	return result;
}

vector<string> getProof(vector<string> variables, vector<string> assumptions)
{
	if (variables.size() != 0) {
		string v = variables[0];
		variables.erase(variables.begin());
		vector<string> list;
		vector<string> copy(assumptions);
		assumptions.push_back(v);
		copy.push_back("!(" + v + ")");
		vector<string> trueCopy(assumptions);
		vector<string> notTrueCopy(copy);
		list << (buildDeductiveProof(assumptions, getProof(variables, trueCopy)));
		list << (buildDeductiveProof(copy, getProof(variables, notTrueCopy)));
		list << (aOrNotA(v));
		list << (aEnd(v));
		return list;
	}
	vector<ExprTree*> insides;
	getOperatorExpressions(PropositionalParser::parse(statement), insides);
	return getBaseProof(insides, getValues(assumptions));
}

template<typename Stream>
void buildProof(Stream& s, vector<string> variablesVec)
{
	s << "|-" << statement << "\n";
	vector<string> emptyAssumptions;
	vector<string> proof = getProof(variablesVec, emptyAssumptions);
	for (int i = 0; i < proof.size(); i++)
	{
		s << proof[i] << endl;
	}
}

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
			return 0;
		}
	}

	buildProof(output_f, variablesVec);


	return 0;
}
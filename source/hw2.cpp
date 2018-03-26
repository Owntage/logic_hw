//
// Created by Owntage on 3/25/2018.
//

#include <iostream>
#include <fstream>
#include "prop_input_parser.h"
#include "propositional_proof.h"

using namespace std;


string implStr(const string& a, const string& b)
{
	string result;
	result += "(";
	result += a;
	result += ")->(";
	result += b;
	result += ")";
	return result;
}

string brackets(const string& a)
{
	string result;
	result += "(";
	result += a;
	result += ")";
	return result;
}

template<typename Stream>
void printAxiom1(Stream& s, string a, string b)
{
	s << brackets(a) << "->" << implStr(b, a) << "\n";
}

template<typename Stream>
void printAxiom2(Stream& s, string a, string b, string c)
{
	s << brackets(implStr(a, b)) << "->" << brackets(implStr(a, implStr(b, c))) << "->" << brackets(implStr(a, c)) << "\n";
}

template<typename Stream>
void printForAxiomOrAssump(Stream& s, string& expr, string& targetExpr)
{
	printAxiom1(s, expr, targetExpr);
	s << expr << "\n";
	s << targetExpr << "->" << expr << "\n";
}

template<typename Stream>
void printForTarget(Stream& s, string& expr, string& t)
{
	printAxiom1(s, t, t);
	string impl = t + "->" + t;
	printAxiom1(s, t, impl);
	printAxiom2(s, t, impl, t);
	s << "(" << t << "->(" << implStr(t, t) << ")->" << t << ")->(" << implStr(t, t) << ")" << endl;
	s << implStr(t, t) << endl;
}

template<typename Stream>
void printForMp(Stream& s, string& expr, string& targetExpr, string& mp1, string& mp2)
{
	printAxiom2(s, targetExpr, mp1, expr);
	s << implStr(implStr(targetExpr, mp2), implStr(targetExpr, expr)) << endl;
	s << implStr(targetExpr, expr) << endl;
}

int main()
{
	string input_filename;
	string output_filename;

	cout << "enter input filename: " << endl;
	cin >> input_filename;

	cout << "enter output filename: " << endl;
	cin >> output_filename;

	string input;
	string targetExpr;
	vector<string> assumptions;
	vector<string> proofExpressions;

	ifstream input_f(input_filename);
	if (input_f)
	{
		while (!input_f.eof())
		{
			input.push_back(input_f.get());
		}
	}

	parsePropositionalInput(input, assumptions, proofExpressions, targetExpr);

	std::cout << "target expression: " << targetExpr;

	PropositionalProofChecker proofChecker(assumptions, proofExpressions);

	ofstream output_f(output_filename);

	for(int i = 0; i < assumptions.size() - 1; i++)
	{
		output_f << assumptions[i];
		if (i != assumptions.size() - 2)
		{
			output_f << ",";
		}
	}
	output_f << "|-" << assumptions.back() << "->" << targetExpr << endl;

	for (int i = 0; i < proofExpressions.size(); i++)
	{
		if (assumptions.back() == proofExpressions[i])
		{
			printForTarget(output_f, proofExpressions[i], assumptions.back());
			continue;
		}
		if (proofChecker.result[i][EXPR_TYPE_KEY] == EXPR_AXIOM ||
				proofChecker.result[i][EXPR_TYPE_KEY] == EXPR_ASSUMPTION)

		{
			printForAxiomOrAssump(output_f, proofExpressions[i], assumptions.back());
			continue;
		}
		if (proofChecker.result[i][EXPR_TYPE_KEY] == EXPR_MP)
		{
			int mpI = proofChecker.result[i][EXPR_MP_FIRST] - 1;
			int mpJ = proofChecker.result[i][EXPR_MP_SECOND] - 1;
			string mpFirst = proofExpressions[mpI];
			string mpSecond = proofExpressions[mpJ];
			printForMp(output_f, proofExpressions[i], assumptions.back(), mpFirst, mpSecond);
		}
	}

	return 0;
}
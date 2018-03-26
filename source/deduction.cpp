//
// Created by Owntage on 3/25/2018.
//

#include "deduction.h"
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



string& operator<<(vector<string>& v, const string& s)
{
	v.push_back(s);
	return v.back();
}

string& operator<<(string& s1, const string& s2)
{
	s1 += s2;
	return s1;
}

void printAxiom1(vector<string>& proof, string a, string b)
{
	proof << brackets(a) << "->" << implStr(b, a);
}

void printAxiom2(vector<string>& proof , string a, string b, string c)
{
	proof << brackets(implStr(a, b)) << "->" << brackets(implStr(a, implStr(b, c))) << "->" << brackets(implStr(a, c));
}

void printForAxiomOrAssump(vector<string>& proof, const string& expr, const string& targetExpr)
{
	printAxiom1(proof, expr, targetExpr);
	proof << expr;
	proof << targetExpr << "->" << expr;
}

void printForTarget(vector<string>& proof, const string& expr, const string& t)
{
	printAxiom1(proof, t, t);
	string impl = t + "->" + t;
	printAxiom1(proof, t, impl);
	printAxiom2(proof, t, impl, t);
	proof << "(" << t << "->(" << implStr(t, t) << ")->" << t << ")->(" << implStr(t, t) << ")";
	proof << implStr(t, t);
}

void printForMp(vector<string>& proof, const string& expr, const string& targetExpr, const string& mp1, const string& mp2)
{
	printAxiom2(proof, targetExpr, mp1, expr);
	proof << implStr(implStr(targetExpr, mp2), implStr(targetExpr, expr));
	proof << implStr(targetExpr, expr);
}

std::vector<std::string> buildDeductiveProof(
		const std::vector<std::string>& assumptions,
		const std::vector<std::string>& proofExpressions
)
{
	PropositionalProofChecker proofChecker(assumptions, proofExpressions);

	vector<string> proof;

	for (int i = 0; i < proofExpressions.size(); i++)
	{
		if (assumptions.back() == proofExpressions[i])
		{
			printForTarget(proof, proofExpressions[i], assumptions.back());
			continue;
		}
		if (proofChecker.result[i][EXPR_TYPE_KEY] == EXPR_AXIOM ||
			proofChecker.result[i][EXPR_TYPE_KEY] == EXPR_ASSUMPTION)

		{
			printForAxiomOrAssump(proof, proofExpressions[i], assumptions.back());
			continue;
		}
		if (proofChecker.result[i][EXPR_TYPE_KEY] == EXPR_MP)
		{
			int mpI = proofChecker.result[i][EXPR_MP_FIRST] - 1;
			int mpJ = proofChecker.result[i][EXPR_MP_SECOND] - 1;
			string mpFirst = proofExpressions[mpI];
			string mpSecond = proofExpressions[mpJ];
			printForMp(proof, proofExpressions[i], assumptions.back(), mpFirst, mpSecond);
		}
	}

	return proof;
}

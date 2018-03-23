//
// Created by Owntage on 3/22/2018.
//

#include <axe.h>
#include <iostream>
#include <stack>

#include "propositional_parser.h"

using namespace axe;

typedef std::string::iterator str_it;

//example from the pdf reference
void extractor_test1()
{
	std::cout << "extractor test 1: " << std::endl;
	std::string input = "an_identifier";
	std::string name;
	size_t length(0);
	auto rule = r_ident() >> name >> e_length(length);
	rule(input.begin(), input.end());
	std::cout << "parsed identifier: " << name
			  << " is " << length << " characters long";
}

template <typename Rule>
void check_rule(Rule rule, const std::string& input)
{
	if (rule(input.begin(), input.end()).matched)
	{
		std::cout << "string " + input + " has matched the rule " + rule.name();
	}
	else
	{
		std::cout << "string " + input + " has NOT matched the rule" + rule.name();
	}
	std::cout << std::endl;
}

void char_test()
{
	std::cout << "char_test" << std::endl;
	auto char_rule = r_char('a');

	std::string input = "a";
	check_rule(char_rule, input);

	input = "aa";
	check_rule(char_rule, input);

	input = "b";
	check_rule(char_rule, input);

	input = "ba";
	check_rule(char_rule, input);

	input = "ab";
	check_rule(char_rule, input);

	input = "aaababa";
	auto char_rule_closure = +char_rule;
	auto result_pos = char_rule(input.begin(), input.end()).position;
	std::cout << "chars consumed with char_rule: " << result_pos - input.begin() << std::endl;
	auto closure_res = char_rule_closure(input.begin(), input.end()).position;
	std::cout << "chars consumed with closure rule: " << closure_res - input.begin() << std::endl;

	auto fail_rule = r_fail([&input](str_it begin, str_it end)
	{
		std::cout << "failed range: " << end - begin << std::endl;
	});
	auto char_with_fail = char_rule_closure | fail_rule;
	input = "baababa";
	auto fail_it = char_with_fail(input.begin(), input.end()).position;
	std::cout << "failed position: " << fail_it - input.begin();
}

void extractor_test2()
{
	std::cout << "extractor test 2: " << std::endl;
	std::string matched_string;
	auto rule = +r_char('a');
	auto rule_with_extractor = rule >> matched_string;
	std::string input = "aaaabbbbaaabba";
	rule_with_extractor(input.begin(), input.end());
	std::cout << "matched string: " << matched_string << std::endl << "applying rule with lambda: " << std::endl;

	auto rule_with_callback = rule >> e_ref([](str_it begin, str_it end)
	{
		std::string matched_str(begin, end);
		std::cout << "matched string: " << matched_str << std::endl;
	});
	rule_with_callback(input.begin(), input.end());

	std::cout << "applying skip rule" << std::endl;
	auto skip_rule = +(rule_with_callback | r_char('b'));
	skip_rule(input.begin(), input.end());
}

template <typename Rule>
void check_expr(Rule rule, std::string input)
{
	auto match_result = rule(input.begin(), input.end());
	if (match_result.matched)
	{
		std::cout << "\"" << input << "\" is an arithmetic expression" << std::endl;
	}
	else
	{
		std::cout << "\"" << input << "\" is NOT an arithmetic expression" << std::endl;
	}
}


void arithmetic_parser()
{
	auto variable = r_any("abc");
	auto constant = r_numstr();
	auto unary_expr = variable | constant;
	auto mulOperations = r_any("*/");
	auto multiplication = r_many(unary_expr, mulOperations);
	auto addOperations = r_any("+-");
	auto addition = r_many(multiplication, addOperations);
	auto expression = addition & r_end();

	std::cout << "testing arithmetic expressions: " << std::endl;
	check_expr(expression, "a");
	check_expr(expression, "a*b");
	check_expr(expression, "abacaba");
	check_expr(expression, "a*b+c");
	check_expr(expression, "a-b+c-b*255");
	check_expr(expression, "");
	check_expr(expression, "a*d");
}

int main()
{
	//extractor_test1();
	//char_test();
	//extractor_test2();
	arithmetic_parser();
	return 0;
}
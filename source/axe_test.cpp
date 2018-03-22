//
// Created by Owntage on 3/22/2018.
//

#include <axe.h>
#include <iostream>

using namespace axe;

//example from the pdf reference
void extractor_test()
{
	std::cout << "extractor test: " << std::endl;
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

	input = "aaaba";
	auto char_rule_closure = +char_rule;
	auto result_pos = char_rule(input.begin(), input.end()).position;
	std::cout << "chars consumed with char_rule: " << result_pos - input.begin() << std::endl;
	auto closure_res = char_rule_closure(input.begin(), input.end()).position;
	std::cout << "chars consumed with closure rule: " << closure_res - input.begin() << std::endl;
}

int main()
{
	extractor_test();
	char_test();

	return 0;
}
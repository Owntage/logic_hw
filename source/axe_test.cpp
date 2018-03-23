//
// Created by Owntage on 3/22/2018.
//

#include <axe.h>
#include <iostream>

using namespace axe;

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

	typedef decltype(input.begin()) str_it;
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
	auto rule = +r_char('a') >> matched_string;
	std::string input = "aaaabbbb";
	rule(input.begin(), input.end());
	std::cout << "matched string: " << matched_string;
}

int main()
{
	//extractor_test1();
	//char_test();
	extractor_test2();
	return 0;
}
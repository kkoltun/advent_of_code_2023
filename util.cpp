#include <iostream>

#include "util.h"

int char_to_int(char c)
{
	return c - '0';
}

unsigned long parse_number(std::string s, size_t* i)
{
	size_t j = *i;
	while (isdigit(s[j]))
	{
		++j;
	}

	if (*i == j)
	{
		return -1;
	}

	unsigned long number = 0;
	while (*i != j)
	{
		int digit = char_to_int(s[*i]);
		int expotent = j - *i - 1;
		number += digit * pow(10, expotent);
		++(*i);
	}

	return number;
}
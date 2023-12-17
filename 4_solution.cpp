#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include "4_solution.hpp"
#include "util.h"

const int MAX = 1000;

void solve_4()
{
	std::ifstream infile("4_input.txt");
	std::stringstream buffer;
	buffer << infile.rdbuf();

	int sum = 0;

	std::string line;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	int card_counts[MAX]{};

	for (int c = 0; std::getline(buffer, line); ++c)
	{
		card_counts[c] += 1;

		bool skip_until_colon = true;
		bool checking_card = false;
		int matches = 0;

		bool winning_idxs[MAX]{};

		for (int i = 0; i < line.length(); ++i)
		{
			if (skip_until_colon && line[i] != ':')
			{
				continue;
			}
			skip_until_colon = false;

			if (isdigit(line[i]))
			{
				int j = i;
				while (isdigit(line[j]))
				{
					++j;
				}

				int number = 0;
				while (i != j)
				{
					int digit = char_to_int(line[i]);
					int expotent = j - i - 1;
					number += digit * pow(10, expotent);
					++i;
				}

				if (checking_card)
				{
					if (winning_idxs[number])
					{
						++matches;
					}
				}
				else
				{
					winning_idxs[number] = true;
				}
			}
			else if (line[i] == '|')
			{
				checking_card = true;
			}
		}

		int current_card_copies = card_counts[c];
		for (int i = 0; i < matches; ++i)
		{
			card_counts[c + i + 1] += current_card_copies;
		}

		int card_result = 0;
		if (matches > 0)
		{
			card_result = 1;
			for (int i = 1; i < matches; ++i)
			{
				card_result *= 2;
			}
		}

		sum += card_result;
	}

	int cards_sum = 0;
	for (int i = 0; i < MAX; ++i)
	{
		cards_sum += card_counts[i];
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Sum: " << sum << "; cards sum: " << cards_sum << std::endl;
	std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
}
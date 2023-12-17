#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include "2_solution.hpp"
#include "util.h"

int max_red_count = 12;
int max_green_count = 13;
int max_blue_count = 14;

void solve_2()
{
	std::ifstream infile("2_input.txt");
	std::stringstream buffer;
	buffer << infile.rdbuf();
	int sum = 0;
	int power_sum = 0;
	std::string line;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (std::getline(buffer, line))
	{
		int game_number = -1;
		int color_count = -1;
		bool is_possible = true;
		bool skip_to_next_number = false;

		int min_red = 0;
		int min_blue = 0;
		int min_green = 0;

		std::cout << std::endl;

		for (int i = 0; i < line.length(); ++i)
		{
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

				if (game_number == -1)
				{
					game_number = number;
					// std::cout << "Game " << game_number << ": ";
				}
				else
				{
					color_count = number;
					// std::cout << color_count << " ";
				}
				skip_to_next_number = false;
				continue;
			}
			else
			{
				if (skip_to_next_number)
				{
					continue;
				}
				char c = line[i];
				if (c == 'r') {
					// std::cout << "red ";
					if (color_count > max_red_count)
					{
						is_possible = false;
					}

					if (color_count > min_red)
					{
						min_red = color_count;
					}

					skip_to_next_number = true;
				}
				if (c == 'b') {
					// std::cout << "blue ";
					if (color_count > max_blue_count)
					{
						is_possible = false;
					}

					if (color_count > min_blue)
					{
						min_blue = color_count;
					}

					skip_to_next_number = true;
				}
				if (c == 'g')
				{
					// `std::cout << "green ";
					if (color_count > max_green_count)
					{
						is_possible = false;
					}

					if (color_count > min_green)
					{
						min_green = color_count;
					}

					skip_to_next_number = true;
				}
			}
		}

		int power = min_red * min_green * min_blue;
		power_sum += power;

		if (is_possible)
		{
			sum += game_number;
		}
	}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Sum: " << sum << "; power: " << power_sum << std::endl;
	std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
}
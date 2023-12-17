#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "3_solution.hpp"
#include "util.h"

const int SIZE = 150;

bool is_include_symbol(char c)
{
	return c != '\0' && !isdigit(c) && c != '.';
}

bool is_safe_idx(size_t i, size_t j)
{
	return i >= 0 && i < SIZE && j >= 0 && j < SIZE;
}

bool is_include_elem(char m[SIZE][SIZE], size_t i, size_t j)
{
	return is_safe_idx(i, j) && is_include_symbol(m[i][j]);
}

bool is_gear_symbol(char m[SIZE][SIZE], size_t i, size_t j)
{
	return is_safe_idx(i, j) && m[i][j] == '*';
}

void record_gear_if_neccessary(char m[SIZE][SIZE], int gears[SIZE][SIZE], int gear_ratios[SIZE][SIZE], int number, size_t i, size_t j)
{
	if (!is_gear_symbol(m, i, j))
	{
		return;
	}

	int gear = gears[i][j];
	if (gear == 0)
	{
		// std::cout << "Potential gear ratio found [" << i << "; " << j << "]: " << number << std::endl;
		gears[i][j] = 1;
		gear_ratios[i][j] = number;
	}
	else if (gear == 1)
	{
		// std::cout << "Gear ratio found [" << i << "; " << j << "]: " << number << "*" << gear_ratios[i][j] << std::endl;

		gears[i][j] = 2;
		gear_ratios[i][j] = gear_ratios[i][j] * number;
	}
	else if (gear == 2)
	{
		// std::cout << "Zeroing gear ratio [" << i << "; " << j << "]: " << gear_ratios[i][j] << std::endl;

		gears[i][j] = -1;
		gear_ratios[i][j] = 0;
	}
}

void solve_3()
{
	std::ifstream infile("3_input.txt");
	std::stringstream buffer;
	buffer << infile.rdbuf();

	char matrix[SIZE][SIZE];
	int gears[SIZE][SIZE];
	int gear_ratios[SIZE][SIZE];

	for (size_t i = 0; i < SIZE; ++i)
	{
		for (size_t j = 0; j < SIZE; ++j)
		{
			matrix[i][j] = '\0';
			gears[i][j] = 0;
			gear_ratios[i][j] = 0;
		}
	}

	std::string line;
	for (size_t i = 0; std::getline(buffer, line); ++i)
	{
		for (size_t j = 0; j < line.length(); ++j)
		{
			matrix[i][j] = line[j];
		}
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	int sum = 0;
	for (size_t i = 0; i < SIZE; ++i)
	{
		if (matrix[i][0] == '\0')
		{
			break;
		}

		for (size_t j = 0; j < SIZE; j++)
		{
			char c = matrix[i][j];

			if (c == '\0')
			{
				break;
			}

			if (!isdigit(c))
			{
				continue;
			}

			// parse digits to number
			size_t d = j;

			size_t k = d;
			while (k < SIZE && isdigit(matrix[i][k]))
			{
				++k;
			}

			int number = 0;
			while (d != k)
			{
				int digit = char_to_int(matrix[i][d]);
				int expotent = k - d - 1;
				number += digit * pow(10, expotent);

				++d;
			}

			// check include and gear
			d = j;
			
			bool include_number = false;
			
			for (int s = -1; s < 2; ++s)
			{
				include_number = include_number || is_include_elem(matrix, i + s, d - 1);
				record_gear_if_neccessary(matrix, gears, gear_ratios, number, i + s, d - 1);
			}

			while (d != k)
			{
				include_number = include_number
					|| is_include_elem(matrix, i - 1, d)
					|| is_include_elem(matrix, i + 1, d);

				record_gear_if_neccessary(matrix, gears, gear_ratios, number, i - 1, d);
				record_gear_if_neccessary(matrix, gears, gear_ratios, number, i + 1, d);

				++d;
			}

			for (int s = -1; s < 2; ++s)
			{
				include_number = include_number || is_include_elem(matrix, i + s, d);
				record_gear_if_neccessary(matrix, gears, gear_ratios, number, i + s, d);
			}

			j = d;

			if (include_number)
			{
				sum += number;
			}
		}
	}

	int gear_ratios_sum = 0;
	for (int i = 0; i < SIZE; ++i)
	{
		for (int j = 0; j < SIZE; ++j)
		{
			if (gears[i][j] == 2)
			{
				gear_ratios_sum += gear_ratios[i][j];
			}
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Sum: " << sum << "; gear ratios sum: " << gear_ratios_sum << std::endl;
	std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
}
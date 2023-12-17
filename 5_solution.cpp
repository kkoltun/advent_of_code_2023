#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>

#include "5_solution.hpp"
#include "util.h"

struct Node {
	unsigned long from;
	unsigned long to;
};

std::ostream& operator << (std::ostream& outs, const Node& n) {
	return outs << "[" << n.from << ", " << n.to <<  "]";
}

const int MAX = 10000;

void solve_5()
{
	std::ifstream infile("5_input.txt");
	std::stringstream buffer;
	buffer << infile.rdbuf();

	std::string line;

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	Node src_arr[MAX]{};
	Node dest_arr[MAX]{};

	size_t nodes_count = 0;

	std::getline(buffer, line);
	for (size_t i = 0; i < line.length(); ++i)
	{
		if (!isdigit(line[i]))
		{
			continue;
		}

		unsigned long seed_from = parse_number(line, &i);
		while (!isdigit(line[++i]))
		{
			continue;
		}

		unsigned long range = parse_number(line, &i);
		unsigned long seed_to = seed_from + range;

		Node seed_node = { seed_from, seed_to };

#ifdef DEBUG
		std::cout << "Seed node: " << seed_node << std::endl;
#endif

		dest_arr[nodes_count] = seed_node;
		++nodes_count;
	}

	if (nodes_count == 0)
	{
		return;
	}

	bool new_map_row = false;
	while (std::getline(buffer, line))
	{
		if (line.length() == 0)
		{
			continue;
		}

		if (isalpha(line[0]))
		{
#ifdef DEBUG
			std::cout << "==========================================================" << std::endl;
			std::cout << "New map: " << line << std::endl;

			std::cout << "Sources: ";
#endif

			for (size_t i = 0; i < nodes_count; ++i)
			{
				src_arr[i] = { dest_arr[i].from, dest_arr[i].to };

#ifdef DEBUG
				std::cout << src_arr[i] << " ";
#endif
			}
#ifdef DEBUG
			std::cout << std::endl;
#endif

			continue;
		}
		else
		{
			unsigned long dest_start = -1;
			unsigned long src_start = -1;
			unsigned long range = -1;

			for (size_t i = 0; i < line.length(); ++i)
			{
				if (!isdigit(line[i]))
				{
					continue;
				}

				unsigned long number = parse_number(line, &i);

				if (dest_start == -1)
				{
					dest_start = number;
				}
				else if (src_start == -1)
				{
					src_start = number;
				}
				else
				{
					range = number;
				}
			}

			unsigned long from = src_start;
			unsigned long to = src_start + range;
		    long diff = dest_start - src_start;

			Node map_node = { from, to };

#ifdef DEBUG
			std::cout << "===" << std::endl;
			std::cout << "Map node " << map_node << " diff " << diff << std::endl;
#endif

			for (int i = 0; i < nodes_count; ++i)
			{
				Node& src = src_arr[i];
				Node& dest = dest_arr[i];

				if (src.from == src.to)
				{
					continue;
				}

#ifdef DEBUG
				std::cout << "Check " << src << std::endl;
#endif

				if (src.from >= to || src.to <= from)
				{
#ifdef DEBUG
					std::cout << "No overlap between [" << src.from << ", " << src.to << "] and [" << from << ", " << to << "]" << std::endl;
#endif
					continue;
				}
				
				unsigned long overlap_from = from > src.from
					? from
					: src.from;
				unsigned long overlap_to = src.to > to
					? to
					: src.to;

				Node overlap_node = { overlap_from, overlap_to };
#ifdef DEBUG
				std::cout << "Overlap between " << src << " and " << map_node << " is " << overlap_node << std::endl;
#endif
				if (overlap_from > src.from)
				{
					Node new_node = { src.from, overlap_from };

					src_arr[nodes_count] = { src.from, overlap_from };
					dest_arr[nodes_count] = { src.from, overlap_from };
					++nodes_count;

					src.from = overlap_from;
					dest.from = overlap_from;

#ifdef DEBUG
					std::cout << "Split right result: " << src << " " << new_node << std::endl;
#endif
				}
				if (overlap_to < src.to)
				{
					Node new_node = { overlap_to, src.to };

					src_arr[nodes_count] = { overlap_to, src.to };
					dest_arr[nodes_count] = { overlap_to, src.to };
					++nodes_count;

					src.to = overlap_to;
					dest.to = overlap_to;

#ifdef DEBUG
					std::cout << "Split left result: " << src << " " << new_node << std::endl;
#endif
				}
				
				dest.from += diff;
				dest.to += diff;

#ifdef DEBUG
				std::cout << "Transfer from " << src << " to " << dest << std::endl;
#endif

				src.from = 0;
				src.to = 0;
			}
		}
	}

	unsigned long min = ULONG_MAX;
	for (size_t i = 0; i < nodes_count; ++i)
	{
		unsigned long dest_from  = dest_arr[i].from;
		if (dest_from < min)
		{
			min = dest_from;
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Min: " << min << std::endl;
	std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
}
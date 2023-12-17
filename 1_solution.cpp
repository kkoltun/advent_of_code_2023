#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "1_solution.hpp"

char get_char_or_else(std::string str, int i, char default_char) {
    return i >= 0 && i < str.length()
        ? str[i]
        : default_char;
}

int find_digit(std::string str, int i, bool forward) {
    if (i < 0 || i >= str.length()) {
        return -1;
    }
    if (isdigit(str[i])) {
        return str[i] - '0';
    }

    int c1_idx = forward ? i : i - 4;
    int c2_idx = forward ? i + 1 : i - 3;
    int c3_idx = forward ? i + 2 : i - 2;
    int c4_idx = forward ? i + 3 : i - 1;
    int c5_idx = forward ? i + 4 : i;

    std::string chars = std::string();
    chars.push_back(get_char_or_else(str, c1_idx, ' '));
    chars.push_back(get_char_or_else(str, c2_idx, ' '));
    chars.push_back(get_char_or_else(str, c3_idx, ' '));
    chars.push_back(get_char_or_else(str, c4_idx, ' '));
    chars.push_back(get_char_or_else(str, c5_idx, ' '));

    if (chars == "three") {
        return 3;
    }
    else if (chars == "seven") {
        return 7;
    }
    else if (chars == "eight") {
        return 8;
    }

    chars.erase(forward ? 4 : 0, 1);

    if (chars == "four") {
        return 4;
    }
    else if (chars == "five") {
        return 5;
    }
    else if (chars == "nine") {
        return 9;
    }

    chars.erase(forward ? 3 : 0, 1);

    if (chars == "one") {
        return 1;
    }
    else if (chars == "two") {
        return 2;
    }
    else if (chars == "six") {
        return 6;
    }

    return -1;
}

void solve_1()
{
    std::ifstream infile("1_input.txt");
    std::stringstream buffer;
    buffer << infile.rdbuf();

    std::string line;
    int sum = 0;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    while (std::getline(buffer, line))
    {
        int last_digit = -1;
        int first_digit = -1;
        for (int i = 0, j = line.length() - 1; i < line.length() && j >= 0 && (first_digit == -1 || last_digit == -1); ++i, --j) {
            if (first_digit == -1) {
                first_digit = find_digit(line, i, true);
            }
            if (last_digit == -1) {
                last_digit = find_digit(line, j, false);
            }
        }

        sum += 10 * first_digit + last_digit;
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
}
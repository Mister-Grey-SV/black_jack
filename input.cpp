#include "input.h"
#include <iostream>

// input
int getNum(const int min, const int max) // get number from user
{
	while (true)
	{
		std::string input;
		std::getline(std::cin, input);

		if (input.empty())
		{
			continue;
		}

		int output{};
		if (tryParse(input, output) && min <= output && output <= max)
		{
			return output;
		}
	}
}

bool tryParse(const std::string& input, int& output) // validation from string to int
{
	size_t position{};

	try
	{
		output = std::stoi(input, &position);
	}

	catch (...)
	{
		return false;
	}

	return position == input.size();
}

void getKey()
{
	std::cin.get();
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
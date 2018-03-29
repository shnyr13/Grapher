#include "IntegerPermutationsGenerator.h"


IntegerPermutationsGenerator::IntegerPermutationsGenerator(int N, int first)
{
	if (N > CombinatoricsGlobals::MAX_SIZE)	throw CombinatoricsGlobals::MAX_SIZE_EXCEPTION;

	if (N < 1)	throw CombinatoricsGlobals::INTEGER_PERMUTATIONS_GENERATOR_SIZE_EXCEPTION;

	std::vector<int> base;
	int value = first;
	for (int i = 1; i <= N; i++) //цикл N раз
	{
		base.push_back(value);
		value++;
	}

	_base = base;
	_COMPARE_FLAG = true;

	generate();
}


IntegerPermutationsGenerator::~IntegerPermutationsGenerator()
{
	_base.clear();
	_result.clear();
}

void IntegerPermutationsGenerator::toFile(std::string filename)
{
	CombinatoricsOutput<int>::toFile(filename, _result);
}

int IntegerPermutationsGenerator::getPermutationNumber(std::vector<int> permutation)
{
	int number = 1;
	for (int i = 1; i < permutation.size(); i++)
	{
		number += (permutation[i - 1] - 1) * CombinatoricsGlobals::factorial(permutation.size() - i);

		if (permutation[i - 1] < permutation.size() - i + 1)
		{
			for (int j = i; j < permutation.size(); j++)
			{

				if (permutation[j] > permutation[i-1])	permutation[j]--;
			}
		}

	}
	return number;
}

std::vector<int> IntegerPermutationsGenerator::getPermutation(int number, int n)
{
	std::vector<int> permutation;	

	bool *used = new bool[n];
	for (int i = 0; i < n; i++)	used[i] = false;

	int i = 1;
	while (permutation.size() != n && i <= n)
	{

		int div = number / CombinatoricsGlobals::factorial(n - i);
		int rest = number % CombinatoricsGlobals::factorial(n - i);

		int element = div;

		if (rest != 0)	element++;

		for (int i = 0; i < element; i++)
		{
			if (used[i])	element++;
		}

		permutation.push_back(element);
		used[element - 1] = true;

		if (rest == 0)
		{
			for (int j = n-1; j >= 0; j--)
			{
				if (!used[j])
				{
					permutation.push_back(j + 1);
					used[j] = true;
				}
			}
		}

		number = rest;

		i++;
	}

	return permutation;
}
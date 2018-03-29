#pragma once
#include "PermutationsGenerator.h"
#include "CombinatoricsOutput.h"

class IntegerPermutationsGenerator :
	public PermutationsGenerator < int >
{
public:
	IntegerPermutationsGenerator(int N, int first = 1);
	~IntegerPermutationsGenerator();

	void toFile(std::string filename);

	static int getPermutationNumber(std::vector<int> permutation);

	static std::vector<int> getPermutation(int number, int n);
	
};


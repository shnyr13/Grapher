#include "IntegerSubsetsGenerator.h"

IntegerSubsetsGenerator::IntegerSubsetsGenerator(unsigned int N, int first)
{
	if (N > CombinatoricsGlobals::MAX_SIZE)	throw CombinatoricsGlobals::MAX_SIZE_EXCEPTION;

	std::vector<int> base;
	int value = first;
	for (int i = 1; i <= N; i++) //цикл N раз
	{
		base.push_back(value);
		value++;
	}

	_base = base;
}
IntegerSubsetsGenerator::~IntegerSubsetsGenerator()
{
	_result.clear();
	_base.clear();
}

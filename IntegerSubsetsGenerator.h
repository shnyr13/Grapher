#pragma once
#include "SubsetsGenerator.h"
class IntegerSubsetsGenerator :
	public SubsetsGenerator < int >
{
public:
	IntegerSubsetsGenerator(unsigned int N, int first = 1);
	~IntegerSubsetsGenerator();
};


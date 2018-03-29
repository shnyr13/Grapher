#include "GlobalDefines.h"

unsigned int CombinatoricsGlobals::factorial(unsigned int a)
{
	// факториалы от 0 до 10 уже определены в библиотеке для повышения эффективности работы
	if (a <= CombinatoricsGlobals::MAX_SIZE)
	{
		return CombinatoricsGlobals::A[a];
	}

	throw CombinatoricsGlobals::MAX_SIZE_EXCEPTION;
}
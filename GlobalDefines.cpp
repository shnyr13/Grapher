#include "GlobalDefines.h"

unsigned int CombinatoricsGlobals::factorial(unsigned int a)
{
	// ���������� �� 0 �� 10 ��� ���������� � ���������� ��� ��������� ������������� ������
	if (a <= CombinatoricsGlobals::MAX_SIZE)
	{
		return CombinatoricsGlobals::A[a];
	}

	throw CombinatoricsGlobals::MAX_SIZE_EXCEPTION;
}
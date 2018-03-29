#pragma once
#include "AbstractGenerator.h"

template <typename T>
class SubsetsGenerator :
	public AbstractGenerator<T>
{
public:
	SubsetsGenerator(std::vector<T> base)
	{
		if (base.size() > MAX_SIZE)	MAX_SIZE_EXCEPTION;
		_base = base;
	}
	~SubsetsGenerator()
	{
	
	}

	virtual void generate()
	{
		unsigned int count = 1;
		for (int i = 0; i < _base.size(); i++)
		{
			count *= 2;
		}
		// Пустое множество
		for (int i = 0; i < count; i++)
		{
			std::vector<unsigned int> containsFlags;
			to2SS(i, containsFlags);

			//заполинить нулями недостающие старшие разряды
			while (containsFlags.size() < _base.size())
			{
				containsFlags.insert(containsFlags.begin(), 0);
			}

			//сформировать подмножество
			std::vector<T> subset;

			for (int i = 0; i < _base.size(); i++)
			{
				if (containsFlags[i])	subset.push_back(_base[i]);
			}
			_result.push_back(subset);

		}
	}

	std::vector<T> _base;

protected:
	SubsetsGenerator(){}

	void to2SS(unsigned int value, std::vector<unsigned int> &result)
	{
		if (value)
		{
			to2SS(value >> 1, result);
			result.push_back(value & 1);
		}
	}
};


#pragma once
#include "AbstractGenerator.h"

template <typename T>
class PermutationsGenerator :
	public AbstractGenerator<T>
{
public:

	PermutationsGenerator(std::vector<T> base, bool COMPARE_FLAG = false)
	{
		if (base.size() > CombinatoricsGlobals::MAX_SIZE)	throw CombinatoricsGlobals::MAX_SIZE_EXCEPTION;
		_base = base;
		_COMPARE_FLAG = COMPARE_FLAG;
	}

	~PermutationsGenerator()
	{

	}

	virtual void generate()
	{
		_result.clear();

		//можно сравнивать элементы, а значит, можно предотвратить повторы
		if (_COMPARE_FLAG)
		{
			//найти первую перестановку
			while (prev_permutation(_base.begin(), _base.end()));
			//if (_base[0] > _base.size() - 1)
			//{
				next_permutation(_base.begin(), _base.end());
			//}

			//генерация всех перестановок (в лексикографическом порядке)
			_result.push_back(_base);
			while (next_permutation(_base.begin(), _base.end()))
			{
				_result.push_back(_base);
			}
		}
		//перестановка с помощью ассоциируемых индексов
		else
		{
			IntegerPermutationsGenerator *ipg = new IntegerPermutationsGenerator(_base.size(), 0);
			ipg->generate();
			std::vector<std::vector<int>> indexesResult = ipg->getResult(); //получить перестановки индексов

			for (int i = 0; i < indexesResult.size(); i++)
			{
				std::vector<T> A;
				for (int j = 0; j < indexesResult[i].size(); j++)
				{
					A.push_back(_base[indexesResult[i][j]]);
				}
				_result.push_back(A);
			}
		}
	}

	void oneTranspositionGenerate()
	{
		_result.clear();

		if (_COMPARE_FLAG)
		{
			std::vector<T> base = _base;
			for (int i = base.size() - 1; i >= 0; i--)
			{
				for (int j = i - 1; j >= 0; j--)
				{
					T temp = base[i];
					base[i] = base[j];
					base[j] = temp;
					_result.push_back(base);
					base = _base;
				}
			}
		}
		else
		{

		}
	}

protected:

	bool _COMPARE_FLAG; //можно ли сравнивать элементы множества?

	PermutationsGenerator(){ }
};
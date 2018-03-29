#pragma once

#include <string>
#include "math.h"

using namespace std;
namespace CombinatoricsGlobals
{

	//максимальный размер множества
	const unsigned int MAX_SIZE = 10;
	const std::string MAX_SIZE_EXCEPTION = "Maximum size is 10";

	const std::string INTEGER_PERMUTATIONS_GENERATOR_SIZE_EXCEPTION = "Invalid size of base multiplicity";

	const std::string INTEGER_PERMUTATIONS_GENERATOR_GET_PERMUTATION_INDEX_EXCEPTION = "Invalid size of index in method \"getPermutation\"";

	const std::string PERMUTATIONS_GENERATOR_GET_NUMBER_COMPARE_EXEPTION = "Permutation can not be compare. If you items can be compare, please create new object of class PermutationsGenerator with COMPARE_FLAG = true. Else you can not call a method \"getNumber\"";
	const std::string PERMUTATIONS_GENERATOR_GET_NUMBER_PERMUTATION_SIZE_EXEPTION = "Permutation size is invalid, because this size not equals size of multiplicity. Please, set right size of input permutation and call this method agan. Else you can not call a method \"getNumber\"";

	const unsigned int A[11] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800 };

	unsigned int factorial(unsigned int a);
};
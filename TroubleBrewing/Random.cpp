#include <cassert>
#include "Random.hpp"

namespace TroubleBrewing
{

int RandomBetween(int a, int b)
{
	assert(a <= b);
	static std::random_device device;
	static std::mt19937 engine{device()};
	std::uniform_int_distribution<decltype(engine)::result_type> distribution(a, b);
	return distribution(engine);
}

int RandomBetweenExclude(int a, int b, int exclude)
{
	assert(a <= exclude && exclude <= b && a <= b);
	int x = RandomBetween(a, b - 1);
	return (x >= exclude) ? (x+1) : x;
}

bool RandomBool()
{
	return RandomBetween(0, 1) == 0;
}

}
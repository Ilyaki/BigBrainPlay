#include "Random.hpp"

int TroubleBrewing::RandomBetweenInc(int a, int b)
{
	static std::random_device device;
	static std::mt19937 engine{device()};
	std::uniform_int_distribution<decltype(engine)::result_type> distribution(a,b);
	return distribution(engine);
}

bool TroubleBrewing::RandomBool()
{
	return RandomBetweenInc(0,1) == 0;
}
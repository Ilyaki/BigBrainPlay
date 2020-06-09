#ifndef BBP_CONSOLE_RANDOM_HPP
#define BBP_CONSOLE_RANDOM_HPP

#include <random>

namespace TroubleBrewing
{

/// \return Random integer a <= x <= b
int RandomBetween(int a, int b);

/// \return Random integer a <= x <= b, x =/= exclude
int RandomBetweenExclude(int a, int b, int exclude);

bool RandomBool();

}

#endif //BBP_CONSOLE_RANDOM_HPP

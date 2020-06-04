#ifndef BBP_CONSOLE_CHARACTERTRAITS_HPP
#define BBP_CONSOLE_CHARACTERTRAITS_HPP

namespace TroubleBrewing
{

struct CharacterTraits
{
	bool isEvil;
	bool isTownsfolk;
	bool isOutsider;
	bool isMinion;
	bool isDemon;

	bool operator==(const CharacterTraits &second) const = default;

	static constexpr CharacterTraits Demon()
	{
		return CharacterTraits {.isEvil =  true, .isTownsfolk =  false, .isOutsider = false, .isMinion = false, .isDemon = true};
	}

	static constexpr CharacterTraits Minion()
	{
		return CharacterTraits {.isEvil =  true, .isTownsfolk =  false, .isOutsider = false, .isMinion = true, .isDemon = false};
	}

	static constexpr CharacterTraits Outsider()
	{
		return CharacterTraits {.isEvil =  false, .isTownsfolk =  false, .isOutsider = true, .isMinion = false, .isDemon = false};
	}

	static constexpr CharacterTraits Townsfolk()
	{
		return CharacterTraits {.isEvil =  false, .isTownsfolk =  true, .isOutsider = false, .isMinion = false, .isDemon = false};
	}
};

// Default == works with C++20. Need this is C++20 doesn't work.
/*bool operator==(const CharacterTraits &first, const CharacterTraits &second)
{
	return	(first.isEvil == second.isEvil) &&
			(first.isTownsfolk == second.isTownsfolk) &&
			(first.isOutsider == second.isOutsider) &&
			(first.isMinion == second.isMinion) &&
			(first.isDemon == second.isDemon);
}*/

}

#endif //BBP_CONSOLE_CHARACTERTRAITS_HPP

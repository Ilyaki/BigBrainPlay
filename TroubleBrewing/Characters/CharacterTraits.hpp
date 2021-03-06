#ifndef BBP_CONSOLE_CHARACTERTRAITS_HPP
#define BBP_CONSOLE_CHARACTERTRAITS_HPP

#include <map>
#include "CharacterTypeGetter.hpp"

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
		return CharacterTraits
			{.isEvil =  true, .isTownsfolk =  false, .isOutsider = false, .isMinion = false, .isDemon = true};
	}

	static constexpr CharacterTraits Minion()
	{
		return CharacterTraits
			{.isEvil =  true, .isTownsfolk =  false, .isOutsider = false, .isMinion = true, .isDemon = false};
	}

	static constexpr CharacterTraits Outsider()
	{
		return CharacterTraits
			{.isEvil =  false, .isTownsfolk =  false, .isOutsider = true, .isMinion = false, .isDemon = false};
	}

	static constexpr CharacterTraits Townsfolk()
	{
		return CharacterTraits
			{.isEvil =  false, .isTownsfolk =  true, .isOutsider = false, .isMinion = false, .isDemon = false};
	}
};

template<CharacterType C>
struct GetCharacterTraitsT
{
	static constexpr CharacterTraits value = GetCharacterType<C>::CharTraits;
};

template<CharacterType C>
static constexpr CharacterTraits GetCharacterTraits() { return GetCharacterTraitsT<C>::value; }

template<typename T>
struct CharacterTypeTraitsPair
{
	using type = T;
	static constexpr CharacterTraits traits = T::CharTraits;
	static constexpr CharacterType characterType = T::CharType;
	static constexpr std::string_view charName = T::CharName;
	static_assert(GetCharacterTraits<characterType>() == traits);
};

using CharacterTypeTraitsMap = std::map<CharacterType, std::pair<CharacterTraits, std::string_view>>;

template<typename... T>
struct GetCharacterTypeTraitMapStruct;

struct GetCharacterTypeTraitMapStructMapBase
{
	CharacterTypeTraitsMap GetMap()
	{
		return map;
	}

protected:
	void Insert(CharacterType a, CharacterTraits b, std::string_view c)
	{
		map.insert({a, {b, c}});
	}

private:
	CharacterTypeTraitsMap map{};
};

template<>
struct GetCharacterTypeTraitMapStruct<> : public virtual GetCharacterTypeTraitMapStructMapBase
{
	GetCharacterTypeTraitMapStruct<>(){}
};

template<typename H, typename... T>
struct GetCharacterTypeTraitMapStruct<H, T...> : public GetCharacterTypeTraitMapStruct<T...>,
        public virtual GetCharacterTypeTraitMapStructMapBase
{
	GetCharacterTypeTraitMapStruct<H, T...>() : GetCharacterTypeTraitMapStruct<T...>()
	{
		using x = CharacterTypeTraitsPair<H>;
		Insert(x::characterType, x::traits, x::charName);
	}
};

template<typename... T>
CharacterTypeTraitsMap GetCharacterTypeTraitMapTemplate()
{
	return GetCharacterTypeTraitMapStruct<T...>{}.GetMap();
}

}

#endif //BBP_CONSOLE_CHARACTERTRAITS_HPP

#ifndef BBP_SAINT_HPP
#define BBP_SAINT_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Saint : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::SAINT;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Outsider();
	static constexpr std::string_view CharName  = "Saint";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS
};

}

#endif //BBP_SAINT_HPP

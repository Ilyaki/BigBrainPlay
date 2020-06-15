#ifndef BBP_BARON_HPP
#define BBP_BARON_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Baron : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::BARON;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Minion();
	static constexpr std::string_view CharName  = "Baron";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS
};

}

#endif //BBP_BARON_HPP

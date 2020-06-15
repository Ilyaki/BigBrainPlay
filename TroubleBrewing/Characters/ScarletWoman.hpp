#ifndef BBP_SCARLETWOMAN_HPP
#define BBP_SCARLETWOMAN_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class ScarletWoman : public Character
{
	static constexpr int numAliveRequired = 5;

public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::SCARLETWOMAN;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Minion();
	static constexpr std::string_view CharName  = "Scarlet Woman";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	bool ScarletWomanCanSwitchIntoDemon(GameState* gameState);
};

}

#endif //BBP_SCARLETWOMAN_HPP

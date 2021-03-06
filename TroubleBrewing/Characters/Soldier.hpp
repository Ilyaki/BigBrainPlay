#ifndef BBP_SOLDIER_HPP
#define BBP_SOLDIER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Soldier : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::SOLDIER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Soldier";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	std::tuple<bool, bool, Player*> AllowCharacterDeath(
			GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr) override;
};

}

#endif //BBP_SOLDIER_HPP

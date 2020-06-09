#ifndef BBP_SOLDIER_HPP
#define BBP_SOLDIER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Soldier : public Character
{
public:
	Soldier(Player* player) : Character(player){}

	static constexpr CharacterType CharType = CharacterType::SOLDIER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Soldier";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	bool AllowCharacterDeath(
			GameState* gamestate, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr) override;
};

}

#endif //BBP_SOLDIER_HPP

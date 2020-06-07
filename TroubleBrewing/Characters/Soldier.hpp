#ifndef BBP_SOLDIER_HPP
#define BBP_SOLDIER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Soldier : public Character
{
public:
	explicit Soldier(Player* _player) :
		Character(_player, "Soldier", CharacterType::SOLDIER, CharacterTraits::Townsfolk()){};

	bool AllowCharacterDeath(
			GameState* gamestate, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr) override;
};

}

#endif //BBP_SOLDIER_HPP

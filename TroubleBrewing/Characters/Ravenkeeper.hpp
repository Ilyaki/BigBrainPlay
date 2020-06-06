#ifndef BBP_RAVENKEEPER_HPP
#define BBP_RAVENKEEPER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Ravenkeeper : public Character
{
public:
	explicit Ravenkeeper(Player* _player);

	std::string GetCharacterName() override;
	CharacterType GetCharacterType() override;

	void OnDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer) override;
};

}

#endif //BBP_RAVENKEEPER_HPP

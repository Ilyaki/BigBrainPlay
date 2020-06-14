#include "Mayor.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

std::tuple<bool, bool, Player*> Mayor::AllowCharacterDeath(
		GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	bool abilityWorks = !player->IsDead() && !gameState->AbilityMalfunctions(player);
	if (!abilityWorks)
		return { true, false, nullptr }; // Allow death
	else
	{
		std::vector<Player*> possibleGood;
		for (Player* p : gameState->GetPlayers())
		{
			if (!p->GetCharacter()->GetCharacterTraits().isEvil && p != player && !p->IsDead())
				possibleGood.push_back(p);
		}

		if (possibleGood.size() == 0 || RandomBetween(3, 10) <= 3)
			// Allow death
			return { true, false, nullptr };
		else
			// Pick someone else to die
			return { true, true, possibleGood.at(RandomBetween(0, possibleGood.size() - 1)) };
	}
}

}
#include "Soldier.hpp"

namespace TroubleBrewing
{

std::tuple<bool, bool, Player*> Soldier::AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	return { !(isDemonKill && !gameState->AbilityMalfunctions(player)), false, nullptr };
}

}

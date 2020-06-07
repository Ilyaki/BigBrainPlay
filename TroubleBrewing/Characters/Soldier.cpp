#include "Soldier.hpp"

namespace TroubleBrewing
{

bool Soldier::AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	return !(isDemonKill && !player->AbilityMalfunctions(gameState));

}


}

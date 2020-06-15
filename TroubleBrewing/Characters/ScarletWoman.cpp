#include "ScarletWoman.hpp"

namespace TroubleBrewing
{

bool ScarletWoman::ScarletWomanCanSwitchIntoDemon(GameState *gameState)
{
	return !player->IsDead() && !gameState->AbilityMalfunctions(player) &&
		   gameState->GetNumPlayersAlive() >= numAliveRequired;
}

}
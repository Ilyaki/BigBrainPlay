#include <cassert>
#include "GameLogging.hpp"

namespace TroubleBrewing
{

Player *GameLogging::GetLastExecutionDeath()
{
	assert(executionDeaths.size() > 0);
	return executionDeaths.back();
}

void GameLogging::SetLastExecutionDeath(Player *player)
{
	executionDeaths.push_back(player);
}

bool GameLogging::HasAnyExecutionPhaseTakenPlace()
{
	executionDeaths.size() != 0;
}

}

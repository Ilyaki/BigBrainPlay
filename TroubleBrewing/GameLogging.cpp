#include <cassert>
#include "GameLogging.hpp"
#include "Time.hpp"

namespace TroubleBrewing
{

std::pair<Player*, Time> GameLogging::GetLastExecutionDeath()
{
	assert(executionDeaths.size() > 0);
	return executionDeaths.back();
}

void GameLogging::SetLastExecutionDeath(Player *player, Time executionTime)
{
	executionDeaths.push_back({player, executionTime});
}

bool GameLogging::HasAnyExecutionPhaseTakenPlace()
{
	return executionDeaths.size() != 0;
}

}

#include <cassert>
#include <ranges>
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

const std::vector<std::pair<Player *, Time>> *GameLogging::GetDeaths()
{
	return &deaths;
}

void GameLogging::LogDeath(Player *player, Time deathTime)
{
	deaths.push_back({player, deathTime});
}

void GameLogging::AddNomination(Player *nominee, Player *nominator, Time time)
{
	nominees.push_back({nominee, time});
	nominators.push_back({nominator, time});
}

bool GameLogging::HasBeenNominee(Player *nominee, Time time)
{
	return 0 != std::ranges::distance(nominees |
		std::views::filter([nominee, time](auto p){ return p.first == nominee && p.second == time; }));
}

bool GameLogging::HasBeenNominator(Player *nominator, Time time)
{
	return 0 != std::ranges::distance(nominators |
		std::views::filter([nominator, time](auto p){ return p.first == nominator && p.second == time; }));
}

}

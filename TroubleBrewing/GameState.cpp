#include <iostream>
#include <functional>
#include <cassert>
#include <ranges>
#include "GameState.hpp"

using namespace TroubleBrewing;

GameState::GameState()
{
}

void GameState::AddPlayer(std::shared_ptr<Player> player)
{
	cacheValid = false;
	players.push_back(player);
}

const std::vector<Player*>& GameState::GetPlayers()
{
	if (!cacheValid)
	{
		playerCache.clear();
		for (decltype(players)::size_type i = 0; i < players.size(); ++i)
		{
			playerCache.push_back(players.at(i).get());
		}
		cacheValid = true;
	}

	return playerCache;
}

int GameState::NumberOfPlayers()
{
	return players.size();
}

std::pair<Player*, Player*> GameState::GetNeighbours(Player* player, bool allowDead)
{
	assert(allowDead || !player->IsDead());

	auto players = GetPlayers();
	auto ptrsView = (players | std::views::filter([allowDead](Player* p)
			{ return allowDead || !p->IsDead(); } ));
	std::vector<Player*> ptrs;
	std::ranges::copy(ptrsView, std::back_inserter(ptrs));
	//auto ptrs = allowDead ? GetPlayers() : GetAlive();
	auto index = std::find(ptrs.begin(), ptrs.end(), player);
	assert(index != ptrs.end());

	if (ptrs.size() <= 1)
		return {nullptr, nullptr};
	else
	{
		Player* left = (index == ptrs.begin()) ? ptrs.back() : *(index - 1);
		Player* right = (index == (ptrs.end() - 1)) ? ptrs.front() : *(index + 1);
		right = (right == left) ? nullptr : right; // Don't give repeated neighbour.
		return {left, right};
	}
}

bool GameState::PlayersAreNeighbours(Player* first, Player* second, bool allowDead)
{
	if (first == second)
		return false;

	auto neighbours = GetNeighbours(first, allowDead);
	return second == neighbours.first || second == neighbours.second;
}

Time GameState::GameStateGetCurrentTime()
{
	return currentTime;
}

void GameState::GameStateSetCurrentTime(Time newTime)
{
	currentTime = newTime;
}

int GameState::GetNumPlayersAlive()
{
	auto aliveView = players | std::views::filter([](auto p){ return !p->IsDead(); } );
	return std::ranges::distance(aliveView);
}

bool GameState::AbilityMalfunctions(Player *player)
{
	return	player->PlayerPartialCheckAbilityMalfunctions(this) ||
			player->GetCharacterOrDrunkBaseCharacter()->IsDrunk();
}

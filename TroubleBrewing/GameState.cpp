#include <iostream>
#include <functional>
#include <cassert>
#include "GameState.hpp"

using namespace TroubleBrewing;

GameState::GameState()
{
}

void GameState::AddPlayer(std::shared_ptr<Player> player)
{
	std::cout << "GameState: Adding " << player->GetCharacter()->GetCharacterName() << std::endl;
	players.push_back(player);
}

#pragma region Player list

std::vector<Player*> GameState::GetPlayers()
{
	//TODO: This should be cached.
	std::vector<Player*> playerPointers;
	for (int i = 0; i < players.size(); ++i)
	{
		playerPointers.push_back(players.at(i).get());
	}

	return playerPointers;
}

int GameState::NumberOfPlayers()
{
	return players.size();
}

std::vector<Player*> GameState::MatchingPlayers(std::function<bool(Player* player)> predicate)
{
	std::vector<Player*> matches;
	for (Player* p : GetPlayers())
	{
		if (predicate(p))
			matches.push_back(p);
	}

	return matches;
}

std::vector<Player*> GameState::GetPlayersMatchingCharacter(CharacterType characterType)
{
	//TODO: We may need to sort these in a consistent order.
	return MatchingPlayers([characterType](Player* p)
		{ return p->GetCharacter()->GetCharacterType() == characterType; });
}

std::vector<Player*> GameState::GetAlive()
{
	return MatchingPlayers([](Player* p){ return !p->IsDead(); });
}

std::vector<Player*> GameState::GetTownsfolk()
{
	return MatchingPlayers([](Player* p){ return p->GetCharacter()->traits.isTownsfolk; });
}

std::vector<Player*> GameState::GetEvil()
{
	return MatchingPlayers([](Player* p){ return p->GetCharacter()->traits.isEvil; });
}

std::vector<Player *> GameState::GetGood()
{
	return MatchingPlayers([](Player* p)
		{ return p->GetCharacter()->traits.isTownsfolk || p->GetCharacter()->traits.isOutsider; });
}

std::vector<Player*> GameState::GetMinions()
{
	return MatchingPlayers([](Player* p){ return p->GetCharacter()->traits.isMinion; });
}

std::vector<Player*> GameState::GetOutsiders()
{
	return MatchingPlayers([](Player* p){ return p->GetCharacter()->traits.isOutsider; });
}

std::vector<Player*> GameState::GetDemons()
{
	return MatchingPlayers([](Player* p){ return p->GetCharacter()->traits.isDemon; });
}

std::pair<Player*, Player*> GameState::GetNeighbours(Player* player, bool allowDead)
{
	assert(allowDead || !player->IsDead());

	auto ptrs = allowDead ? GetPlayers() : GetAlive();
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

Time GameState::GetCurrentTime()
{
	return currentTime;
}

void GameState::SetCurrentTime(Time newTime)
{
	currentTime = newTime;
}

#pragma endregion


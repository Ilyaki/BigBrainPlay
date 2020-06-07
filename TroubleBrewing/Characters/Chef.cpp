#include <iostream>
#include "Chef.hpp"

namespace TroubleBrewing
{

Chef::Chef(Player *_player) : Character(_player, "Chef", CharacterType::CHEF, CharacterTraits::Townsfolk())
{

}

void Chef::NightAction(bool zerothNight, GameState *gameState)
{
	// TODO: drunk/poisoned

	if (!zerothNight)
		return;

	// Go through each evil player, checking how many other evil are sitting next to them.
	// Only count a pair if the other evil player has not already been checked.
	// After checking an evil player, mark them as counted.

	int totalPairs = 0;
	auto evilPlayers = gameState->GetEvil();
	std::vector<Player*> countedEvilPlayers{};

	for (Player* checking : evilPlayers)
	{
		for (Player* other : evilPlayers)
		{
			if (checking != other
				&& gameState->PlayersAreNeighbours(checking, other)
				&& (countedEvilPlayers.empty() || std::find(countedEvilPlayers.begin(), countedEvilPlayers.end(), other) == countedEvilPlayers.end())
			)
			{
				// Evil neighbour that hasn't been counted.
				++totalPairs;
			}
		}

		countedEvilPlayers.push_back(checking);
	}

	// Communicate to player.
	//TODO: send proper message
	std::string msg = GetCharacterName() + " ability: There are ";
	msg += std::to_string(totalPairs);
	msg += " evil pairs";
	player->Communication()->SendMessage(msg);
}

}
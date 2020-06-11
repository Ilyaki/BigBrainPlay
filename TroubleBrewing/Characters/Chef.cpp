#include <iostream>
#include <ranges>
#include "Chef.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

void Chef::NightAction(bool zerothNight, GameState *gameState)
{
	if (!zerothNight)
		return;

	// Go through each evil player, checking how many other evil are sitting next to them.
	// Only count a pair if the other evil player has not already been checked.
	// After checking an evil player, mark them as counted.

	int totalPairs = 0;

	if (gameState->AbilityMalfunctions(player))
	{
		//TODO: maybe revisit this (Chef Drunk)
		totalPairs = RandomBool() ? 1 : 0;
	}
	else
	{
		auto players = gameState->GetPlayers();
		auto evilPlayers = players | std::views::filter(
				[](Player *p) { return p->GetCharacter()->GeneratePerceivedTraits().isEvil; });
		std::vector<Player *> countedEvilPlayers{};

		for (Player *checking : evilPlayers)
		{
			for (Player *other : evilPlayers)
			{
				if (checking != other
					&& gameState->PlayersAreNeighbours(checking, other)
					&& (countedEvilPlayers.empty() ||
						std::find(countedEvilPlayers.begin(), countedEvilPlayers.end(), other) ==
						countedEvilPlayers.end())
						)
				{
					// Evil neighbour that hasn't been counted.
					++totalPairs;
				}
			}

			countedEvilPlayers.push_back(checking);
		}
	}

	// Communicate to player.
	player->Communication()->SendMessage(GetCharacterNameString() + " ability: There are " +
		std::to_string(totalPairs) + " evil pairs");
}

}
#include "Empath.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

void Empath::NightAction(bool zerothNight, GameState *gameState)
{
	// Counts how many neighbours are evil

	int evilNeighbours = 0;

	if (gameState->AbilityMalfunctions(player))
	{
		evilNeighbours = RandomBool() ? (RandomBool() ? 2 : 1) : 0;
	}
	else
	{
		auto neighbours = gameState->GetNeighbours(player, false);

		if (neighbours.first != nullptr &&
				neighbours.first->GetCharacter()->GeneratePerceivedCharacterData(gameState)
													.externalPerceivedCharacterTraits.isEvil)
			++evilNeighbours;

		if (neighbours.second != nullptr &&
				neighbours.second->GetCharacter()->GeneratePerceivedCharacterData(gameState)
													.externalPerceivedCharacterTraits.isEvil)
			++evilNeighbours;
	}

	// Communicate to player
	player->Communication()->SendMessage(GetCharacterNameString() + ": You have " +
		std::to_string(evilNeighbours) + " evil neighbours");
}

}

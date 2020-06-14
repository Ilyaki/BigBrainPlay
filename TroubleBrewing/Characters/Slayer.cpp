#include "Slayer.hpp"

namespace TroubleBrewing
{

Player* Slayer::AttemptSlay(Player *target, GameState* gameState)
{
	if (!player->IsDead() &&
		!gameState->AbilityMalfunctions(player) && // Slay always fails if drunk/poisoned
		target->GetCharacter()->GeneratePerceivedCharacterData(gameState)
									.externalPerceivedCharacterTraits.isDemon) // Can kill a Recluse
	{
		return target->AttemptKill(gameState, false, false, player);
	}
	else
		return nullptr;
}

}

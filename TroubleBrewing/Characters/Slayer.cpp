#include "Slayer.hpp"

namespace TroubleBrewing
{

bool Slayer::AttemptSlay(Player *target, GameState* gameState)
{
	return	!gameState->AbilityMalfunctions(player) && // Slay always fails if drunk/poisoned
			target->GetCharacter()->GeneratePerceivedTraits().isDemon &&
			target->AttemptKill(gameState, false, false, player);
}

}

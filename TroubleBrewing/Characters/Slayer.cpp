#include "Slayer.hpp"

namespace TroubleBrewing
{

bool Slayer::AttemptSlay(Player *target, GameState* gameState)
{
	return	!player->AbilityMalfunctions(gameState) && // Slay always fails if drunk/poisoned
			target->GetCharacter()->traits.isDemon && // TODO: "perceived" as demon because recluse
			target->AttemptKill(gameState, false, false, player);
}

}

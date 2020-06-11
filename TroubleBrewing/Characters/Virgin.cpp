#include "Virgin.hpp"

namespace TroubleBrewing
{

bool Virgin::VirginAbility(Player *nominator, GameState* gameState)
{
	if (usedVirginAbility)
		return false;
	else
	{
		usedVirginAbility = true;
		return gameState->AbilityMalfunctions(player) && player->GetCharacter()->GeneratePerceivedTraits().isTownsfolk;
	}
}

}
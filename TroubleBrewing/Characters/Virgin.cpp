#include "Virgin.hpp"

namespace TroubleBrewing
{

bool Virgin::VirginAbility(Player *nominator, GameState* gameState)
{
	if (usedVirginAbility || player->IsDead())
		return false;
	else
	{
		usedVirginAbility = true;
		return !gameState->AbilityMalfunctions(player) && player->GetCharacter()
				->GeneratePerceivedCharacterData(gameState).externalPerceivedCharacterTraits.isTownsfolk;
	}
}

}
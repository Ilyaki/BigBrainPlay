#include "Virgin.hpp"

namespace TroubleBrewing
{

bool Virgin::VirginAbility(Player *nominator)
{
	if (usedVirginAbility)
		return false;
	else
	{
		//TODO: need 'perceived' as townsfolk for the Spy ability
		usedVirginAbility = true;
		return player->GetCharacter()->GetCharacterTraits().isTownsfolk;
	}
}

}
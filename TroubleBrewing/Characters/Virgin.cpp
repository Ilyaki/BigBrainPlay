#include "Virgin.hpp"

TroubleBrewing::Virgin::Virgin(TroubleBrewing::Player *_player) :
	Character(_player, "Virgin", CharacterType::VIRGIN, CharacterTraits::Townsfolk())
{
}

bool TroubleBrewing::Virgin::VirginAbility(Player* nominator)
{
	if (usedVirginAbility)
		return false;
	else
	{
		//TODO: need 'perceived' as townsfolk for the Spy ability
		usedVirginAbility = true;
		return player->GetCharacter()->traits.isTownsfolk;
	}
}

#include "Virgin.hpp"

TroubleBrewing::Virgin::Virgin(TroubleBrewing::Player *_player) : Character(_player, CharacterTraits::Townsfolk())
{
}

std::string TroubleBrewing::Virgin::GetCharacterName()
{
	return "Virgin";
}

CharacterType TroubleBrewing::Virgin::GetCharacterType()
{
	return CharacterType::VIRGIN;
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

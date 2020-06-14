#include "Character.hpp"

namespace TroubleBrewing
{

bool Character::AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	return true;
}

void Character::AnnounceCharacterAndAlignment()
{
	player->Communication()->SendMessage("You are the " +
				player->GetCharacterOrDrunkBaseCharacter()->GetCharacterNameString() +
				", your alignment is " + (player->GetCharacterOrDrunkBaseCharacter()->GetCharacterTraits().isEvil ?
				"Evil" : "Good"));
}

CharacterTraits Character::GeneratePerceivedTraits()
{
	return GetCharacterTraits();
}

CharacterType Character::GetSelfPerceivedCharacter()
{
	return GetCharacterType();
}

bool Character::IsDrunk() const
{
	return isDrunk;
}

Time Character::GetCreationTime() const
{
	return creationTime;
}

}
#include "Character.hpp"

namespace TroubleBrewing
{

bool Character::AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	return true;
}

void Character::InitialSetup(GameState *gameState)
{
	player->Communication()->SendMessage("You are the " + GetCharacterNameString() +
		", your alignment is " + (GetCharacterTraits().isEvil ? "Evil" : "Good"));
}

CharacterTraits Character::GeneratePerceivedTraits()
{
	return GetCharacterTraits();
}

CharacterType Character::GetPerceivedCharacter()
{
	return GetCharacterType();
}

bool Character::IsDrunk() const
{
	return isDrunk;
}

}
#include "Character.hpp"

namespace TroubleBrewing
{

bool Character::AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	return true;
}

void Character::InitialSetup(GameState *gameState)
{
	player->Communication()->SendMessage("You are the " + GetCharacterName() +
		", your alignment is " + (traits.isEvil ? "Evil" : "Good"));
}

std::string Character::GetCharacterName() const
{
	return characterName;
}

CharacterType Character::GetCharacterType() const
{
	return characterType;
}

}
#include "Undertaker.hpp"

namespace TroubleBrewing
{

void Undertaker::NightAction(bool zerothNight, GameState *gameState)
{
	//TODO: drunk/poisoned

	if (zerothNight)
		return;

	Player *exec = gameState->GetLastExecutionDeath();
	if (exec == nullptr)
		player->Communication()->SendMessage(GetCharacterNameString() + ": No character was executed last night");
	else
	{
		player->Communication()->SendMessage(
				GetCharacterNameString() + ": The " + exec->GetCharacter()->GetCharacterNameString() + " was executed last night");
	}
}

}
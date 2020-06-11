#include "Monk.hpp"

namespace TroubleBrewing
{

void Monk::NightAction(bool zerothNight, GameState *gameState)
{
	if (zerothNight)
		return;

	Player* target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose a player to protect from the demon",
			{
				[this](Player* selection){ return selection != this->player; },
				"You cannot choose yourself"
			}
			);

	// Needs to be after the input, or they'd know they were the drunk
	if (IsDrunk()) // In case we are the Drunk, don't override the actual Monk protection
		return;

	// Monk protection only lasts the current night
	// Note: This *will* set the monk protection if poisoned, which is important when poison wears off
	target->SetMonkProtection(gameState->GetCurrentTime(), player);
}

}
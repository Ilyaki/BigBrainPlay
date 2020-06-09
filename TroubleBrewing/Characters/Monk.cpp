#include "Monk.hpp"

namespace TroubleBrewing
{

void Monk::NightAction(bool zerothNight, GameState *gameState)
{
	if (zerothNight)
		return;

	if (player->AbilityMalfunctions(gameState)) // In case Drunk, don't override the actual Monk protection
		return;

	Player* target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose a player to protect from the demon",
			{
				[this](Player* selection){ return selection != this->player; },
				"You cannot choose yourself"
			}
			);

	// Monk protection only lasts the current night
	target->SetMonkProtection(gameState->GetCurrentTime(), player);
}

}
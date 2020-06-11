#include "Poisoner.hpp"

namespace TroubleBrewing
{

void Poisoner::NightAction(bool zerothNight, GameState *gameState)
{
	Player* target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose who you want to poison");

	auto currentTime = gameState->GetCurrentTime();
	auto until = Time { true, currentTime.DayOrNightCount() + 1 }; // Next day

	if (IsDrunk()) // Can't actually have a Drunk Poisoner, but just in case we change the rules
		return;

	target->SetPoisoned(until, player);
}

}
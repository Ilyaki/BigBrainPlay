#include "Poisoner.hpp"

namespace TroubleBrewing
{

void Poisoner::NightAction(bool zerothNight, GameState *gameState)
{
	Player* target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose who you want to poison");

	auto currentTime = gameState->GameStateGetCurrentTime();
	auto until = Time { true, currentTime.DayOrNightCount() + 1 }; // Next day

	// Note we aren't checking for Poisoned. If we are poisoned, we do want to apply the effect (it just does nothing)
	if (IsDrunk()) // Can't actually have a Drunk Poisoner, but just in case we change the rules
		return;

	target->SetPoisoned(until, player);
}

}
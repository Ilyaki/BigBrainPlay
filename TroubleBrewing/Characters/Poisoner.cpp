#include "Poisoner.hpp"

namespace TroubleBrewing
{

void Poisoner::NightAction(bool zerothNight, GameState *gameState)
{
	Player* target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose who you want to poison");

	auto currentTime = gameState->GetCurrentTime();
	auto until = Time { true, currentTime.DayOrNightCount() + 1 }; // Next day

	target->SetPoisoned(until, player);
}

}
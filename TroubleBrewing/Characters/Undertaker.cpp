#include "Undertaker.hpp"

TroubleBrewing::Undertaker::Undertaker(TroubleBrewing::Player *_player) :
	Character(_player, "Undertaker", CharacterType::UNDERTAKER, CharacterTraits::Townsfolk())
{
}

void TroubleBrewing::Undertaker::NightAction(bool zerothNight, TroubleBrewing::GameState *gameState)
{
	//TODO: drunk/poisoned

	if (zerothNight)
		return;

	Player* exec = gameState->GetLastExecutionDeath();
	if (exec == nullptr)
		player->Communication()->SendMessage(GetCharacterName() + ": No character was executed last night");
	else
	{
		player->Communication()->SendMessage(
				GetCharacterName() + ": The " + exec->GetCharacter()->GetCharacterName() + " was executed last night");
	}
}
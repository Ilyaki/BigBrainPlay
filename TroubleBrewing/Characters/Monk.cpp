#include "Monk.hpp"

namespace TroubleBrewing
{

Monk::Monk(Player *_player) : Character(_player, CharacterTraits::Townsfolk())
{
}

std::string Monk::GetCharacterName()
{
	return "Monk";
}

CharacterType Monk::GetCharacterType()
{
	return CharacterType::MONK;
}

void Monk::NightAction(bool zerothNight, GameState *gameState)
{
	//TODO: drunk/poisoned

	if (zerothNight)
		return;

	Player* target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterName() + ": Choose a player to protect from the demon",
			{
				[this](Player* selection){ return selection != this->player; },
				"You cannot choose yourself"
			}
			);

	// Monk protection only lasts the current night
	target->SetMonkProtection(gameState->GetCurrentTime(), player);
}

}
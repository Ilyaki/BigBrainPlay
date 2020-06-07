#include "Ravenkeeper.hpp"

namespace TroubleBrewing
{

Ravenkeeper::Ravenkeeper(Player *_player) :
	Character(_player, "Ravenkeeper", CharacterType::RAVENKEEPER, CharacterTraits::Townsfolk())
{
}

void Ravenkeeper::OnDeath(GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	//TODO: be careful not to call this during demon information,
	// or the demon will have to wait for text and it would give the Ravenkeeper away
	//TODO: drunk/poisoned

	if (!gameState->GetCurrentTime().IsDay())
	{
		Player *target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterName() + ": You have died. Choose a player to learn their character");

		player->Communication()->SendMessage(GetCharacterName() + ": " + target->PlayerName() +
			" is a " + target->GetCharacter()->GetCharacterName());
	}
}

}
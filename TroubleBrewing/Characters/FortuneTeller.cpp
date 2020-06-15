#include <iostream>
#include "FortuneTeller.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

void FortuneTeller::InitialSetup(GameState *gameState)
{
	Character::InitialSetup(gameState);

	// Choose a Red Herring
	std::vector<Player*> redHerringCandidates;
	if (RandomBetween(1, 5) == 1)
		redHerringCandidates = gameState->GetPlayers(); // Consider all players, including minions/demons
	else
	{
		auto players = gameState->GetPlayers();

		std::ranges::copy(players | std::views::filter([](Player* p)
									{ return !p->GetCharacter()->GetCharacterTraits().isEvil; }), // Get actual traits
				std::back_inserter(redHerringCandidates));

		// Remove possibility of our own player being the red herring
		if (gameState->NumberOfPlayers() > 6 || RandomBool())
		{
			redHerringCandidates.erase(
					std::find(redHerringCandidates.begin(), redHerringCandidates.end(), player));
		}
	}

	if (redHerringCandidates.size() == 0)
		redHerringCandidates = gameState->GetPlayers();

	redHerring = redHerringCandidates.at(RandomBetween(0, redHerringCandidates.size() - 1));
}

void FortuneTeller::NightAction(bool zerothNight, GameState *gameState)
{
	auto firstPlayer = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose the first player to check");

	auto secondPlayer = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose the second player to check",
			{[firstPlayer](Player* p){ return p != firstPlayer; },
					"You must choose different players"});

	bool fortuneTellerReading;

	if (gameState->AbilityMalfunctions(player))
		fortuneTellerReading = RandomBetween(1, 5) <= 2;
	else
	{
		fortuneTellerReading = firstPlayer == redHerring || secondPlayer == redHerring ||

			   firstPlayer->GetCharacter()->GeneratePerceivedCharacterData(gameState)
			   			.externalPerceivedCharacterTraits.isDemon ||

				secondPlayer->GetCharacter()->GeneratePerceivedCharacterData(gameState)
						.externalPerceivedCharacterTraits.isDemon;
	}

	player->Communication()->SendMessage(
			GetCharacterNameString() + ": Your reading is " + (fortuneTellerReading ? "true" : "false"));
}

}

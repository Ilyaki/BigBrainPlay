#include <iostream>
#include "FortuneTeller.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

FortuneTeller::FortuneTeller(Player *_player) : Character(_player, CharacterTraits::Townsfolk())
{
}

std::string FortuneTeller::GetCharacterName()
{
	return "Fortune Teller";
}

CharacterType FortuneTeller::GetCharacterType()
{
	return CharacterType::FORTUNETELLER;
}

void FortuneTeller::InitialSetup(GameState *gameState)
{
	Character::InitialSetup(gameState);

	// Choose a Red Herring
	std::vector<Player*> redHerringCandidates;
	if (RandomBetweenInc(1, 5) == 1) // TODO: maybe don't hard code red herring chance?
		redHerringCandidates = gameState->GetPlayers(); // Consider all players, including minions/demons
	else
	{
		redHerringCandidates = gameState->GetGood();

		// Remove possibility of our own player being the red herring
		if (gameState->NumberOfPlayers() > 6 || RandomBool())
		{
			redHerringCandidates.erase(
					std::find(redHerringCandidates.begin(), redHerringCandidates.end(), player));
		}
	}

	if (redHerringCandidates.size() == 0)
		redHerringCandidates = gameState->GetPlayers();

	redHerring = redHerringCandidates.at(RandomBetweenInc(0, redHerringCandidates.size()-1));

	std::cout << "Fortune teller red herring = " << redHerring->PlayerName() << std::endl;
}

void FortuneTeller::NightAction(bool zerothNight, GameState *gameState)
{
	// TODO: drunk/poisoned

	auto firstPlayer = player->Communication()->InputPlayer(gameState, player,
			GetCharacterName() + ": Choose the first player to check");

	//TODO: Allow the same player if there is only 1 player? (Can't actually happen in game, only useful in testing)
	auto secondPlayer = player->Communication()->InputPlayer(gameState, player,
			GetCharacterName() + ": Choose the second player to check",
			{[firstPlayer](Player* p){ return p != firstPlayer; },
					"You must choose different players"});

	bool fortuneTellerReading = firstPlayer == redHerring || secondPlayer == redHerring ||
			firstPlayer->GetCharacter()->traits.isDemon || secondPlayer->GetCharacter()->traits.isDemon;

	std::string msg = GetCharacterName() + ": Your reading is " + (fortuneTellerReading ? "true" : "false");
	player->Communication()->SendMessage(msg);
}

}

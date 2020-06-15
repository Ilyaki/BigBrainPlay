#include "Ravenkeeper.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

void Ravenkeeper::PreDeath(GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	//TODO: be careful not to call this during demon information,
	// or the demon will have to wait for text and it would give the Ravenkeeper away

	if (!gameState->GetCurrentTime().IsDay() && !player->IsDead()) // Don't give info if they have already died
	{
		Player *target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": You have died. Choose a player to learn their character");

		std::string targetCharacterName;

		if (gameState->AbilityMalfunctions(player))
		{
			auto map = gameState->GetCharacterTypeTraitsMap();

			// Get character types that match the target & aren't our own character
			auto possibleView = *map | std::views::transform([](auto y){ return y.second.second; });

			// Copy into a vector for processing
			std::vector<std::string_view> possibleCharacterNames;
			std::copy(possibleView.begin(), possibleView.end(), std::back_inserter(possibleCharacterNames));

			if (possibleCharacterNames.empty())
				targetCharacterName = GetCharacterNameString();
			else
				targetCharacterName = std::string{
					possibleCharacterNames.at(RandomBetween(0, possibleCharacterNames.size() - 1)) };

		}
		else
			targetCharacterName = std::string {
				target->GetCharacter()->GeneratePerceivedCharacterData(gameState).externalPerceivedName };

		player->Communication()->SendMessage(GetCharacterNameString() + ": " + target->PlayerName() +
											 " is a " + targetCharacterName);
	}
}

}
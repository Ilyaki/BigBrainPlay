#include "Ravenkeeper.hpp"
#include "../Random.hpp"
#include "CharacterMap.hpp"

namespace TroubleBrewing
{

void Ravenkeeper::PreDeath(GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	if (!gameState->GameStateGetCurrentTime().IsDay() && !player->IsDead()) // Don't give info if they have already died
	{
		Player *target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": You have died. Choose a player to learn their character");

		std::string targetCharacterName;

		if (gameState->AbilityMalfunctions(player))
		{
			// Copy into a vector for processing
			std::vector<std::string_view> possibleCharacterNames;
			std::ranges::copy(*CharacterMap::GetCharacterTypeTraitsMap() |
				std::views::transform([](auto y){ return y.second.second; }),
				std::back_inserter(possibleCharacterNames));

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
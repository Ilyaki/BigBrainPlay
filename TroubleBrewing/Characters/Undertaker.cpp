#include "Undertaker.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

void Undertaker::NightAction(bool zerothNight, GameState *gameState)
{
	if (zerothNight)
		return;

	auto [ exec, execTime ] = gameState->GetLastExecutionDeath();
	if (exec == nullptr)
		player->Communication()->SendMessage(GetCharacterNameString() + ": No character was executed yesterday");
	else
	{
		std::string charName;

		if (gameState->AbilityMalfunctions(player))
		{
			auto map = gameState->GetCharacterTypeTraitsMap();

			// Get character types that aren't our own character
			auto ourCharacter = GetCharacterType();
			std::vector<std::string_view> possible;
			std::ranges::copy(*map	| std::views::filter([ourCharacter](auto y)
														{ return y.first != ourCharacter; })
									| std::views::transform([](auto y) { return y.second.second; }),
									std::back_inserter(possible));

			if (possible.size() == 0)
				charName = GetCharacterName();
			else {
				charName = std::string { possible.at(RandomBetween(0, possible.size() - 1)) };
			}
		}
		else
			charName = std::string { exec->GetCharacter()->
									GeneratePerceivedCharacterData(gameState).externalPerceivedName };

		player->Communication()->SendMessage(
				GetCharacterNameString() + ": The " + charName + " was executed last night");
	}
}

}
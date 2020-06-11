#include <iostream>
#include <ranges>
#include "CharacterKnowerBase.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

void CharacterKnowerBase::NightAction(bool zerothNight, GameState *gameState)
{
	if (!zerothNight)
		return;

	Player *firstPlayer;
	Player *secondPlayer;
	CharacterType knownCharacter;
	std::string knownCharacterName;

	if (gameState->AbilityMalfunctions(player))
	{
		using charPair = std::pair<CharacterType, std::pair<CharacterTraits, std::string_view>>;
		auto targetTraits = TargetTraits();
		auto ourCharacter = GetCharacterType();

		auto map = gameState->GetCharacterTypeTraitsMap();

		// Get character types that match the target & aren't our own character
		auto possibleView = *map | std::views::filter([targetTraits, ourCharacter](charPair y)
									{ return y.second.first == targetTraits && y.first != ourCharacter; })
							| std::views::transform([](charPair y){ return std::make_pair(y.first, y.second.second); });

		// Copy into a vector for processing
		std::vector<std::pair<CharacterType, std::string_view>> possibleCharacters;
		std::ranges::copy(possibleView, std::back_inserter(possibleCharacters));

		if (possibleCharacters.empty())
		{
			knownCharacter = ourCharacter;
			knownCharacterName = GetCharacterName();
		} else {
			auto index = possibleCharacters.at(RandomBetween(0, possibleCharacters.size() - 1));
			knownCharacter = index.first;
			knownCharacterName = std::string{ index.second };
		}

		auto players = gameState->GetPlayers();
		if (players.size() <= 1)
			firstPlayer = secondPlayer = player;
		else
		{
			// Get any 2 random players
			auto firstIndex = RandomBetween(0, players.size() - 1);
			auto secondIndex = RandomBetweenExclude(0, players.size() - 1, firstIndex);
			firstPlayer = players.at(firstIndex);
			secondPlayer = players.at(secondIndex);
		}
	}
	else
	{
		// Get targets that aren't also our character
		auto targetTraits = TargetTraits();
		auto ourCharacter = GetCharacterType();

		auto players = gameState->GetPlayers();
		auto targetsView = players | std::views::filter([targetTraits, ourCharacter](Player* p)
									{ return	p->GetCharacter()->GeneratePerceivedTraits() == targetTraits &&
												p->GetCharacter()->GetCharacterType() != ourCharacter; });

		// Copy into a vector for processing
		std::vector<Player*> targets;
		std::copy(targetsView.begin(), targetsView.end(), std::back_inserter(targets));

		if (targets.size() == 0)
		{
			// No target
			firstPlayer = secondPlayer = nullptr;
			knownCharacter = CharacterType::NO_CHARACTER;
		} else
		{
			// Choose a random player as the first player, and use their character.
			const int firstIndex = RandomBetween(0, targets.size() - 1);
			firstPlayer = targets.at(firstIndex);
			knownCharacter = firstPlayer->GetCharacter()->GetCharacterType();
			knownCharacterName = firstPlayer->GetCharacter()->GetCharacterName();

			// Choose a different random player (from all players except ourselves) for the second player.
			auto allPlayers = gameState->GetPlayers();
			int secondIndex;
			while (allPlayers.at(secondIndex = RandomBetween(0, allPlayers.size() - 1)) == player);
			secondPlayer = allPlayers.at(secondIndex);

			// Shuffle the first/second player so the first one isn't always correct.
			if (RandomBool())
			{
				auto tmp = firstPlayer;
				firstPlayer = secondPlayer;
				secondPlayer = tmp;
			}
		}
	}

	// Communicate to player.
	player->Communication()->SendMessage(GetCharacterNameString() + ": One of " + firstPlayer->PlayerName() +
		" and " + secondPlayer->PlayerName() + " is a " + knownCharacterName);
}

}
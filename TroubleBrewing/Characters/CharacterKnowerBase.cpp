#include <iostream>
#include <ranges>
#include "CharacterKnowerBase.hpp"
#include "../Random.hpp"
#include "CharacterMap.hpp"

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

	const auto ourCharacter = GeneratePerceivedCharacterData(gameState).selfPerceivedCharacterType;

	if (gameState->AbilityMalfunctions(player))
	{
		using charPair = std::pair<CharacterType, std::pair<CharacterTraits, std::string_view>>;
		auto targetTraits = TargetTraits();

		// Get character types that match the target & aren't our own character
		std::vector<std::pair<CharacterType, std::string_view>> possibleCharacters;
		for (charPair p : *CharacterMap::GetCharacterTypeTraitsMap())
		{
			if (p.second.first == targetTraits && p.first != ourCharacter)
				possibleCharacters.push_back({p.first, p.second.second});
		}

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

		std::vector<std::pair<Player*, PerceivedCharacterData>> targets;
		for (Player* p : gameState->GetPlayers())
		{
			if (auto perceived = p->GetCharacter()->GeneratePerceivedCharacterData(gameState);
				perceived.externalPerceivedCharacterTraits == targetTraits &&
				p->GetCharacter()->GetCharacterType() != ourCharacter)
			{
				targets.push_back({p, perceived});
			}
		}

		if (targets.size() == 0)
		{
			// No target
			firstPlayer = secondPlayer = nullptr;
			knownCharacter = CharacterType::NO_CHARACTER;
		} else
		{
			// Choose a random player as the first player, and use their character.
			const int firstIndex = RandomBetween(0, targets.size() - 1);
			auto data = targets.at(firstIndex);
			firstPlayer = data.first;
			auto perceivedData = data.second;
			knownCharacter = perceivedData.externalPerceivedCharacterType;
			knownCharacterName = std::string { perceivedData.externalPerceivedName };

			// Choose a different random player (from all players except the first player & ourselves) for the second player.
			auto allPlayers = gameState->GetPlayers();
			while ((secondPlayer = allPlayers.at(RandomBetween(0, allPlayers.size() - 1))),
					(secondPlayer == firstPlayer || secondPlayer == player)); // Loop again if this is true

			// Shuffle the first/second player so the first one isn't always correct.
			if (RandomBool())
			{
				auto tmp { firstPlayer };
				firstPlayer = secondPlayer;
				secondPlayer = tmp;
			}
		}
	}

	// Communicate to player.
	if (knownCharacter == CharacterType::NO_CHARACTER)
	{
		player->Communication()->SendMessage(GetCharacterNameString() + ": There are zero outsiders in play");
	}
	else
		player->Communication()->SendMessage(GetCharacterNameString() + ": One of " + firstPlayer->PlayerName() +
			" and " + secondPlayer->PlayerName() + " is a " + knownCharacterName);
}

}
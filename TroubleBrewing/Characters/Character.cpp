#include <random>
#include <algorithm>
#include "Character.hpp"
#include "CharacterMap.hpp"

namespace TroubleBrewing
{

std::tuple<bool, bool, Player*> Character::AllowCharacterDeath(
		GameState* gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	return { true, false, nullptr };
}

void Character::AnnounceCharacterAndAlignment(GameState* gameState, bool gameStart, bool teensyville)
{
	auto perceived = GeneratePerceivedCharacterData(gameState);

	player->Communication()->SendMessage("You are the " + std::string { perceived.selfPerceivedName } +
				", your alignment is " + (perceived.selfPerceivedCharacterTraits.isEvil ? "Evil" : "Good"), false);

	// If we add Drunk evil (ie Lunatic), this needs to change
	if (gameStart && GetCharacterTraits().isEvil)
	{
		if (teensyville)
			player->Communication()->SendMessage("Playing by Teensyville rules: you don't know the other evil players");
		else
		{
			if (GetCharacterTraits().isMinion)
			{
				// Minion gets to know the Demon
				for (Player* p : gameState->GetPlayers())
					if (p->GetCharacter()->GetCharacterTraits().isDemon)
						player->Communication()->SendMessage("Your Demon is " + p->PlayerName());
			}
			else
			{
				// Demon gets to know all evil
				for (Player* p : gameState->GetPlayers())
					if (p != player && p->GetCharacter()->GetCharacterTraits().isEvil)
						player->Communication()->SendMessage(
								p->GetCharacter()->GetCharacterTraits().isDemon ?
								"Your friendly Demon is " + p->PlayerName() :
								"Your Minion is " + p->PlayerName());
			}
		}

		if (GetCharacterTraits().isEvil)
			ShowDemonBluff(gameState);
	}
}

void Character::ShowDemonBluff(GameState *gameState)
{
	// Get characters in play
	std::vector<CharacterType> charactersInPlay{};
	for (Player* p : gameState->GetPlayers())
		charactersInPlay.push_back(p->GetCharacter()->GetCharacterType());

	// Get all characters
	std::vector<std::tuple<CharacterType, CharacterTraits, std::string_view>> allCharacters{};
	for (const auto&[charType, pair] : *CharacterMap::GetCharacterTypeTraitsMap())
		allCharacters.push_back({charType, pair.first, pair.second});

	// Randomly sort characters
	std::random_device rng;
	std::mt19937 mtrng { rng() };
	std::shuffle(allCharacters.begin(), allCharacters.end(), mtrng);

	// Choose 2 townsfolk and 1 outsider, or 3 townsfolk if no available outsiders
	std::string_view chosen[3];
	int index = 0;
	bool foundOutsider = false;
	for (const auto&[charType, traits, name ] : allCharacters)
	{
		if (std::ranges::find(charactersInPlay, charType) == std::ranges::end(charactersInPlay)) // Not in play
		{
			// Insert at last index if we haven't found an outsider
			if (traits.isTownsfolk && (!foundOutsider || index != 2))
				chosen[index++] = name;
			else if (traits.isOutsider && charType != CharacterType::DRUNK)
			{
				chosen[2] = name;
				foundOutsider = true;
			}
		}
	}

	// Print out bluffs
	player->Communication()->SendMessage("Your three safe bluffs are " + std::string{chosen[0]} +
		", " + std::string{chosen[1]} + " and " + std::string{chosen[2]});
}

bool Character::IsDrunk() const
{
	return isDrunk;
}

Time Character::GetCreationTime() const
{
	return creationTime;
}

PerceivedCharacterData Character::GeneratePerceivedCharacterData(GameState* gameState)
{
	return {
		.selfPerceivedCharacterTraits = GetCharacterTraits(),
		.selfPerceivedCharacterType = GetCharacterType(),
		.selfPerceivedName = GetCharacterName(),

		.externalPerceivedCharacterTraits = GetCharacterTraits(),
		.externalPerceivedCharacterType = GetCharacterType(),
		.externalPerceivedName = GetCharacterName()
	};
}

}
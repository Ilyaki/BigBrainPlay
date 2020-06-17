#include <random>
#include <algorithm>
#include <cassert>
#include "CharacterChooser.hpp"
#include "CharacterMap.hpp"

namespace TroubleBrewing::CharacterChooser
{

std::tuple<bool, std::vector<CharacterType>, CharacterCount> GenerateCharacterTypes(
		const std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>> &playerDatas)
{
	constexpr auto minPlayers = 5;
	constexpr auto maxPlayers = 15;

	// Source: https://bra1n.github.io/townsquare/
	constexpr CharacterCount counts[] = {
			CharacterCount {3, 0, 1, 1}, // minPlayers players (ie 5 players)
			CharacterCount {3, 1, 1, 1},
			CharacterCount {5, 0, 1, 1},
			CharacterCount {5, 1, 1, 1},
			CharacterCount {5, 2, 1, 1},
			CharacterCount {7, 0, 2, 1},
			CharacterCount {7, 1, 2, 1},
			CharacterCount {7, 2, 2, 1},
			CharacterCount {9, 0, 3, 1},
			CharacterCount {9, 1, 3, 1},
			CharacterCount {9, 2, 3, 1}
	};

	std::vector<CharacterType> chosen {};

	auto numPlayers = playerDatas.size();

	if (numPlayers < minPlayers || numPlayers > maxPlayers) // Not enough players
		return { false, chosen, CharacterCount{0,0,0,0} };

	CharacterCount count = counts[numPlayers - minPlayers];
	const CharacterCount constCount { count };

	// Get all characters
	std::vector<std::pair<CharacterType, CharacterTraits>> allCharacters{};
	for (const auto&[charType, pair] : *CharacterMap::GetCharacterTypeTraitsMap())
		allCharacters.push_back({charType, pair.first});

	// Randomly sort characters
	std::random_device rng;
	std::mt19937 mtrng { rng() };
	std::shuffle(allCharacters.begin(), allCharacters.end(), mtrng);

	// Take the first matching quota
	std::vector<CharacterTraits> chosenTraits; // Same order as chosen
	for (const auto&[ charType, traits ] : allCharacters)
	{
		if ((traits.isTownsfolk && --count.townsfolk >= 0) ||
			(traits.isOutsider && --count.outsiders >= 0) ||
			(traits.isMinion && --count.minions >= 0) ||
			(traits.isDemon && --count.demons >= 0))
		{
			chosen.push_back(charType);
			chosenTraits.push_back(traits);
		}

	}

	// Baron: swaps 2 townsfolk with outsiders
	for (auto charType : chosen)
	{
		if (charType == CharacterType::BARON)
		{
			// Take the first two townsfolk and replace them with two outsiders not already in game

			// Taking outsiders from the back of allCharacters will give two not in game
			auto outsidersIter = (allCharacters	| std::views::reverse
												| std::views::filter([](auto c){ return c.second.isOutsider; })
												| std::views::take(2)).begin();
			CharacterType outsider1 = (*outsidersIter).first;
			CharacterType outsider2 = (*(++outsidersIter)).first;
			assert(outsider1 != outsider2);

			bool secondSwap = false;
			for (size_t i = 0; i < chosen.size(); ++i)
			{
				if (chosenTraits.at(i).isTownsfolk)
				{
					if (secondSwap)
					{
						chosen[i] = outsider2;
						break;
					}
					else
					{
						chosen[i] = outsider1;
						secondSwap = true;
					}
				}
			}

			break;
		}
	}

	return { true, chosen, constCount };
}

}

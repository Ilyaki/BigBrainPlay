#include <cassert>
#include "Spy.hpp"
#include "../Random.hpp"
#include "CharacterMap.hpp"

namespace TroubleBrewing
{

PerceivedCharacterData Spy::GeneratePerceivedCharacterData(GameState *gameState)
{
	// Note: can generate random, even if dead
	if (gameState->AbilityMalfunctions(player) || RandomBetween(1, 3) == 1)
	{
		return Character::GeneratePerceivedCharacterData(gameState); // Default
	}
	else
	{
		// Generate a Townsfolk or Outsider
		bool genTownsfolk = RandomBetween(1, 5) > 1;

		std::vector<std::tuple<CharacterType, CharacterTraits, std::string_view>> possible{};

		for (auto p : *CharacterMap::GetCharacterTypeTraitsMap())
		{
			if (genTownsfolk ? p.second.first.isTownsfolk : p.second.first.isOutsider)
				possible.push_back({p.first, p.second.first, p.second.second});
		}

		assert(possible.size() != 0);

		auto [ extCharType, extCharTraits, extName ] = possible.at(RandomBetween(0, possible.size() - 1));

		// Only change the external view
		return {
				.selfPerceivedCharacterTraits = GetCharacterTraits(),
				.selfPerceivedCharacterType = GetCharacterType(),
				.selfPerceivedName = GetCharacterName(),

				.externalPerceivedCharacterTraits = extCharTraits,
				.externalPerceivedCharacterType = extCharType,
				.externalPerceivedName = extName
		};
	}
}

void Spy::NightAction(bool zerothNight, GameState *gameState)
{
	// If Drunk/Poisoned, the Spy is allowed to see accurate info (phew)
	auto comm = player->Communication();
	comm->SendMessage("GRIMOIRE START:", false);

	for (Player* p : gameState->GetPlayers())
	{
		if (p != player)
		{
			auto perceived = p->GetCharacter()->GeneratePerceivedCharacterData(gameState);
			bool isEvil = perceived.externalPerceivedCharacterTraits.isEvil;
			bool abilityMalfunction = gameState->AbilityMalfunctions(p);

			comm->SendMessage(p->PlayerName() + " is the " + (isEvil ? "Evil " : "Good ") +
					std::string { perceived.externalPerceivedName } +
					(abilityMalfunction ? ". Their ability is malfunctioning" : ""), false);
		}
	}

	comm->SendMessage("GRIMOIRE END", true);
}

}
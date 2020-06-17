#include <cassert>
#include "Recluse.hpp"
#include "../Random.hpp"
#include "CharacterMap.hpp"

namespace TroubleBrewing
{

PerceivedCharacterData Recluse::GeneratePerceivedCharacterData(GameState* gameState)
{
	// Note: can generate random, even if dead
	if (gameState->AbilityMalfunctions(player) || RandomBool())
	{
		return Character::GeneratePerceivedCharacterData(gameState); // Default
	}
	else
	{
		// Generate a Minion or Demon
		bool genMinion = RandomBool();

		std::vector<std::tuple<CharacterType, CharacterTraits, std::string_view>> possible{};

		for (auto p : *CharacterMap::GetCharacterTypeTraitsMap())
		{
			if (genMinion ? p.second.first.isMinion : p.second.first.isDemon)
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
}

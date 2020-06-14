#include <cassert>
#include "Recluse.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

PerceivedCharacterData Recluse::GeneratePerceivedCharacterData(GameState* gameState)
{
	if (gameState->AbilityMalfunctions(player) || RandomBool())
	{
		return Character::GeneratePerceivedCharacterData(gameState); // Default
	}
	else
	{
		// Generate a Minion or Outsider
		bool genMinion = RandomBool();

		std::vector<std::tuple<CharacterType, CharacterTraits, std::string_view>> possible{};

		for (auto p : *gameState->GetCharacterTypeTraitsMap())
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

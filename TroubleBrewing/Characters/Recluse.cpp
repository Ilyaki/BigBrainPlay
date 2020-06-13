#include <cassert>
#include "Recluse.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

CharacterTraits Recluse::GeneratePerceivedTraits()
{
	if (RandomBool())
		return GetCharacterTraits(); // Our actual character traits
	else
	{
		// Generate Minion or Demon traits
		return RandomBool() ? CharacterTraits::Demon() : CharacterTraits::Minion();
	}
}

/*CharacterType Recluse::GetPerceivedCharacter(GameState* gameState)
{
	if (RandomBool())
		return GetCharacterType(); // Our actual character
	else
	{
		// Generate Minion or Demon traits
		bool genMinion = RandomBool();

		std::vector<CharacterType> possible{};

		for (auto p : *gameState->GetCharacterTypeTraitsMap())
		{
			if (genMinion ? p.second.first.isMinion : p.second.first.isDemon)
				possible.push_back(p.first);
		}

		assert(possible.size() != 0);

		auto charType = possible.at(RandomBetween(0, possible.size() - 1));

		return charType;
	}
}*/

}

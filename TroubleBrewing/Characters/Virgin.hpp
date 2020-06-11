#ifndef BBP_VIRGIN_HPP
#define BBP_VIRGIN_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Virgin : public Character
{
	bool usedVirginAbility;

public:
	Virgin(Player* player, bool isDrunk) : Character(player, isDrunk){}

	static constexpr CharacterType CharType = CharacterType::VIRGIN;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Virgin";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	/// VirginAbility: Should be called whenever the Virgin is nominated.
	/// \returns True if the nominator should be executed
	bool VirginAbility(Player* nominator, GameState* gameState);
};

}

#endif //BBP_VIRGIN_HPP

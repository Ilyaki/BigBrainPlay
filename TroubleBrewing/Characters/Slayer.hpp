#ifndef BBP_SLAYER_HPP
#define BBP_SLAYER_HPP

#include "Character.hpp"
#include "../Player.hpp"

namespace TroubleBrewing
{

class Slayer : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::SLAYER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Slayer";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	/// \return true if the target was slain
	bool AttemptSlay(Player *target, GameState *gameState);
};

}


#endif //BBP_SLAYER_HPP

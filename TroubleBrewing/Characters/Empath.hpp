#ifndef BBP_CONSOLE_EMPATH_HPP
#define BBP_CONSOLE_EMPATH_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Empath : public Character
{
public:
	Empath(Player* player, bool isDrunk) : Character(player, isDrunk){}

	static constexpr CharacterType CharType = CharacterType::EMPATH;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Empath";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}


#endif //BBP_CONSOLE_EMPATH_HPP

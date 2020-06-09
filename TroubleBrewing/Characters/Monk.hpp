#ifndef BBP_MONK_HPP
#define BBP_MONK_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Monk : public Character
{
public:
	Monk(Player* player) : Character(player){}

	static constexpr CharacterType CharType = CharacterType::MONK;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Monk";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_MONK_HPP

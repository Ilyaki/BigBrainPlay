#ifndef BBP_UNDERTAKER_HPP
#define BBP_UNDERTAKER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Undertaker : public Character
{
public:
	Undertaker(Player* player, bool isDrunk) : Character(player, isDrunk){}

	static constexpr CharacterType CharType = CharacterType::UNDERTAKER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Undertaker";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_UNDERTAKER_HPP

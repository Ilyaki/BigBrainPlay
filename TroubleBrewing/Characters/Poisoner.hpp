#ifndef BBP_POISONER_HPP
#define BBP_POISONER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Poisoner : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::POISONER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Minion();
	static constexpr std::string_view CharName  = "Poisoner";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}


#endif //BBP_POISONER_HPP

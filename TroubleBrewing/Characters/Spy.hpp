#ifndef BBP_SPY_HPP
#define BBP_SPY_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Spy : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::SPY;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Minion();
	static constexpr std::string_view CharName  = "Spy";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void NightAction(bool zerothNight, GameState* gameState) override;

	PerceivedCharacterData GeneratePerceivedCharacterData(GameState* gameState) override;
};

}

#endif //BBP_SPY_HPP

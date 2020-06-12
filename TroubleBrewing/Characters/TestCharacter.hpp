#ifndef BBP_CONSOLE_TESTCHARACTER_HPP
#define BBP_CONSOLE_TESTCHARACTER_HPP

#include <string>
#include "../GameState.hpp"
#include "Character.hpp"

namespace TroubleBrewing
{

class TestCharacter : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::TEST_CHARACTER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Demon();
	static constexpr std::string_view CharName  = "Test Character";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS


	void InitialSetup(GameState* gameState) override;
	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_CONSOLE_TESTCHARACTER_HPP

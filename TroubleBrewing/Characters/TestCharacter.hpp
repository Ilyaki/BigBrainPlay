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
	explicit TestCharacter(Player* _player);

	void InitialSetup(GameState* gameState) override;
	void NightAction(bool zerothNight, GameState* gameState) override;

	std::string GetCharacterName() override;
	CharacterType GetCharacterType() override;
};

}

#endif //BBP_CONSOLE_TESTCHARACTER_HPP

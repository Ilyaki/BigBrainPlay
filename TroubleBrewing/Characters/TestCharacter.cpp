#include "TestCharacter.hpp"
#include <iostream>

using namespace TroubleBrewing;

TestCharacter::TestCharacter(Player* _player) :
	Character{_player, "Test Character", CharacterType::TEST_CHARACTER, CharacterTraits::Demon()}
{
}

void TestCharacter::InitialSetup(GameState* gameState)
{
	Character::InitialSetup(gameState);

	std::cout << "TestCharacter InitialSetup" << std::endl;
}

void TestCharacter::NightAction(bool zerothNight, GameState* gameState)
{
	std::cout << "TestCharacter night action, zerothNight = " << zerothNight << std::endl;
}


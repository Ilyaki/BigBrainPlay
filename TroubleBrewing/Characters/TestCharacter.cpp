#include "TestCharacter.hpp"
#include <iostream>

namespace TroubleBrewing
{

void TestCharacter::InitialSetup(GameState *gameState)
{
	Character::InitialSetup(gameState);

	std::cout << "TestCharacter InitialSetup" << std::endl;
}

void TestCharacter::NightAction(bool zerothNight, GameState *gameState)
{
	std::cout << "TestCharacter night action, zerothNight = " << zerothNight << std::endl;
}

}

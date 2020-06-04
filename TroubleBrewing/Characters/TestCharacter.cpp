#include "TestCharacter.hpp"
#include <iostream>

using namespace TroubleBrewing;

TestCharacter::TestCharacter(Player* _player) : Character{_player, CharacterTraits::Demon()}
{
}

void TestCharacter::InitialSetup(GameState* gameState)
{
	std::cout << "TestCharacter InitialSetup" << std::endl;
}

void TestCharacter::NightAction(bool zerothNight, GameState* gameState)
{
	std::cout << "TestCharacter night action, zerothNight = " << zerothNight << std::endl;
}

std::string TestCharacter::GetCharacterName()
{
	return "Test Character";
}

CharacterType TestCharacter::GetCharacterType()
{
	return CharacterType::TEST_CHARACTER;
}



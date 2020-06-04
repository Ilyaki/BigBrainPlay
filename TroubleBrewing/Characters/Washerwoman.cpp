#include "Washerwoman.hpp"

using namespace TroubleBrewing;

Washerwoman::Washerwoman(Player* _player) : CharacterKnowerBase(_player, CharacterTraits::Townsfolk())
{
}

std::string Washerwoman::GetCharacterName()
{
	return "Washerwoman";
}

CharacterType Washerwoman::GetCharacterType()
{
	return CharacterType::WASHERWOMAN;
}
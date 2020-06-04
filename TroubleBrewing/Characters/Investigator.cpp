#include "Investigator.hpp"

using namespace TroubleBrewing;

Investigator::Investigator(Player* _player) : CharacterKnowerBase(_player, CharacterTraits::Townsfolk())
{
}

std::string Investigator::GetCharacterName()
{
	return "Investigator";
}

CharacterType Investigator::GetCharacterType()
{
	return CharacterType::INVESTIGATOR;
}
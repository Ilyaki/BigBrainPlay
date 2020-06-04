#include "Librarian.hpp"

using namespace TroubleBrewing;

Librarian::Librarian(Player* _player) : CharacterKnowerBase(_player, CharacterTraits::Townsfolk())
{
}

std::string Librarian::GetCharacterName()
{
	return "Librarian";
}

CharacterType Librarian::GetCharacterType()
{
	return CharacterType::LIBRARIAN;
}
#include "Librarian.hpp"

using namespace TroubleBrewing;

Librarian::Librarian(Player* _player) :
	CharacterKnowerBase(_player, "Librarian", CharacterType::LIBRARIAN, CharacterTraits::Townsfolk())
{
}
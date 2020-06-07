#include "Investigator.hpp"

using namespace TroubleBrewing;

Investigator::Investigator(Player* _player) :
	CharacterKnowerBase(_player, "Investigator", CharacterType::INVESTIGATOR, CharacterTraits::Townsfolk())
{
}
#include "Washerwoman.hpp"

using namespace TroubleBrewing;

Washerwoman::Washerwoman(Player* _player) :
	CharacterKnowerBase(_player, "Washerwoman", CharacterType::WASHERWOMAN, CharacterTraits::Townsfolk())
{
}
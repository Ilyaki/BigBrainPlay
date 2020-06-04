#include "Empath.hpp"

namespace TroubleBrewing
{

Empath::Empath(Player *_player) : Character(_player, CharacterTraits::Townsfolk())
{
}

void Empath::NightAction(bool zerothNight, GameState *gameState)
{
	// TODO: drunk/poisoned

	// Counts how many neighbours are evil

	int evilNeighbours = 0;
	auto neighbours = gameState->GetNeighbours(player, false);

	if (neighbours.first != nullptr && neighbours.first->GetCharacter()->traits.isEvil) //TODO: recluse 'perceived character'/etc
		++evilNeighbours;

	if (neighbours.second != nullptr && neighbours.second->GetCharacter()->traits.isEvil)
		++evilNeighbours;

	// Communicate to player
	//TODO: proper message.
	std::string msg = GetCharacterName() + " ability: You have ";
	msg += std::to_string(evilNeighbours);
	msg += " evil neighbours";
	player->Communication()->SendMessage(msg);
}

std::string Empath::GetCharacterName()
{
	return "Empath";
}

CharacterType Empath::GetCharacterType()
{
	return CharacterType::EMPATH;
}

}

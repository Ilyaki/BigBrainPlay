#include <cassert>
#include <algorithm>
#include "Drunk.hpp"
#include "CharacterHelper.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

void Drunk::InitialSetup(GameState *gameState)
{
	// Do this in InitialSetup so we know what other characters are in play

	// Get possible townsfolk that aren't our own character
	auto ourCharacter = GetCharacterType();
	std::vector<CharacterType> possible;

	for (auto y : *gameState->GetCharacterTypeTraitsMap())
	{
		if (y.first != ourCharacter && y.second.first.isTownsfolk)
			possible.push_back(y.first);
	}

	assert(possible.size() != 0);

	// Find what townsfolk are already existing to avoid a role clash
	std::vector<CharacterType> townsfolkInPlay;
	for (Player* p : gameState->GetPlayers())
	{
		if (auto charType = p->GetCharacter()->GetCharacterType();

			p->GetCharacter()->GetCharacterTraits().isTownsfolk &&
			std::find(townsfolkInPlay.begin(), townsfolkInPlay.end(), charType) == townsfolkInPlay.end())
		{
			townsfolkInPlay.push_back(charType);
			std::remove(possible.begin(), possible.end(), charType);
		}
	}

	auto p = possible.size() == 0 ? &townsfolkInPlay : &possible;

	perceivedCharacterType = p->at(RandomBetween(0, p->size() - 1));
	perceivedCharacter = CharacterHelper::CreateCharacter(perceivedCharacterType, gameState, player, true);
}

CharacterType Drunk::GetSelfPerceivedCharacter()
{
	return perceivedCharacterType;
}

void Drunk::NightAction(bool zerothNight, GameState *gameState)
{
	perceivedCharacter->NightAction(zerothNight, gameState);
}

Character *Drunk::DrunkBaseCharacter()
{
	return perceivedCharacter.get();
}

}
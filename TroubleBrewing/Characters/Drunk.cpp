#include <cassert>
#include "Drunk.hpp"
#include "CharacterHelper.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

Drunk::Drunk(Player *player, GameState* gameState) : Character(player, true)
{
	auto map = gameState->GetCharacterTypeTraitsMap();

	// Get townsfolk that aren't our own character
	auto ourCharacter = GetCharacterType();
	std::vector<CharacterType> possible;
	std::ranges::copy(*map	| std::views::filter([ourCharacter](auto y)
												  { return y.first != ourCharacter && y.second.first.isTownsfolk; })
					  		| std::views::transform([](auto y) { return y.first; }),
					  	std::back_inserter(possible));

	assert(possible.size() != 0);

	perceivedCharacterType = possible.at(RandomBetween(0, possible.size() - 1));
	perceivedCharacter = CharacterHelper::CreateCharacter(perceivedCharacterType, gameState, player, true);
}

CharacterType Drunk::GetPerceivedCharacter()
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
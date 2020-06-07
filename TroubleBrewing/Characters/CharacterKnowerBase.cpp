#include <iostream>
#include "CharacterKnowerBase.hpp"
#include "../Random.hpp"

namespace TroubleBrewing
{

CharacterKnowerBase::CharacterKnowerBase(Player *_player, CharacterTraits traits) : Character(_player, traits)
{

}

void CharacterKnowerBase::NightAction(bool zerothNight, GameState *gameState)
{
	//TODO: drunk/poisoned

	if (!zerothNight)
		return;

	// Get targets that aren't also our character
	auto targetTraits = TargetTraits();
	auto ourCharacter = GetCharacterType();

	//TODO: recluse can show up as a minion. Should add a check for 'perceived character'
	std::vector<Player *> targets = gameState->MatchingPlayers([targetTraits, ourCharacter](Player *p) {
		return ((p->GetCharacter()->traits == targetTraits) && p->GetCharacter()->GetCharacterType() != ourCharacter);
	});

	if (targets.size() == 0)
	{
		// No target
		firstPlayer = secondPlayer = nullptr;
		knownCharacter = CharacterType::NO_CHARACTER;
	} else {
		// Choose a random player as the first player, and use their character.
		const int firstIndex = RandomBetweenInc(0, targets.size() - 1);
		firstPlayer = targets.at(firstIndex);
		knownCharacter = firstPlayer->GetCharacter()->GetCharacterType();

		// Choose a different random player (from all players except ourselves) for the second player.
		auto allPlayers = gameState->GetPlayers();
		int secondIndex;
		while (allPlayers.at(secondIndex = RandomBetweenInc(0, allPlayers.size() - 1)) == player);
		secondPlayer = allPlayers.at(secondIndex);

		// Shuffle the first/second player so the first one isn't always correct. //TODO: Remember to do this for other roles.
		if (RandomBool())
		{
			auto tmp = firstPlayer;
			firstPlayer = secondPlayer;
			secondPlayer = tmp;
		}
	}

	// Communicate to player.
	//TODO: send proper message
	std::string msg = GetCharacterName() + " ability: Someone is a ";
	msg += std::to_string((int) knownCharacter);
	player->Communication()->SendMessage(msg);
}

}
#include "Character.hpp"

namespace TroubleBrewing
{

std::tuple<bool, bool, Player*> Character::AllowCharacterDeath(
		GameState* gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	return { true, false, nullptr };
}

void Character::AnnounceCharacterAndAlignment()
{
	//TODO: perceivedData
	player->Communication()->SendMessage("You are the " +
				player->GetCharacterOrDrunkBaseCharacter()->GetCharacterNameString() +
				", your alignment is " + (player->GetCharacterOrDrunkBaseCharacter()->GetCharacterTraits().isEvil ?
				"Evil" : "Good"));
}

bool Character::IsDrunk() const
{
	return isDrunk;
}

Time Character::GetCreationTime() const
{
	return creationTime;
}

PerceivedCharacterData Character::GeneratePerceivedCharacterData(GameState* gameState)
{
	return {
		.selfPerceivedCharacterTraits = GetCharacterTraits(),
		.selfPerceivedCharacterType = GetCharacterType(),
		.selfPerceivedName = GetCharacterName(),
		.externalPerceivedCharacterTraits = GetCharacterTraits(),
		.externalPerceivedCharacterType = GetCharacterType()
	};
}

}
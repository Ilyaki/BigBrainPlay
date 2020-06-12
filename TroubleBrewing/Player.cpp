#include <iostream>
#include <cassert>
#include "Characters/CharacterHelper.hpp"
#include "Player.hpp"
#include "Characters/Drunk.hpp"

namespace TroubleBrewing
{

Player::Player(CharacterType characterType,
			   PlayerData _playerData,
			   std::shared_ptr<PlayerCommunication> _communication,
			   GameState* gameState) :
		communication{_communication},
		playerData{_playerData},
		character{CharacterHelper::CreateCharacter(characterType, gameState, this, false)}
{
	assert(_playerData.playerID >= 0);
}

std::shared_ptr<Character> Player::GetCharacter()
{
	return character;
}

std::shared_ptr<PlayerCommunication> Player::Communication()
{
	return communication;
}

bool Player::IsDead() const
{
	return isDead;
}

bool Player::AttemptKill(GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	if (isDead)
	{
		Communication()->SendMessage("You have died. Again");
		return true;
	} else
	{
		if (character->AllowCharacterDeath(gameState, isExecutionKill, isDemonKill, sourcePlayer))
		{
			bool monkProtected = (isDemonKill && monkProtectedUntil >= gameState->GetCurrentTime() &&
								  monkProtectedBy != nullptr && !gameState->AbilityMalfunctions(monkProtectedBy));

			if (monkProtected)
				return false;
			else
			{
				Kill(gameState, isExecutionKill, isDemonKill, sourcePlayer);
				return true;
			}
		} else
		{
			return false;
		}
	}
}

bool Player::MatchesPlayerID(int playerID) const
{
	return playerData.playerID == playerID;
}

std::string Player::PlayerName() const
{
	return playerData.playerName;
}

int Player::PlayerID() const
{
	return playerData.playerID;
}

bool Player::PlayerPartialCheckAbilityMalfunctions(GameState *gameState) const
{
	auto currentTime = gameState->GetCurrentTime();
	return poisonedUntil >= currentTime && poisonedBy != nullptr &&
		(this == poisonedBy || !gameState->AbilityMalfunctions(poisonedBy));
}

void Player::SetPoisoned(Time until, Player *fromWho)
{
	poisonedUntil = until;
	poisonedBy = fromWho;
}

void Player::SetMonkProtection(Time until, Player *fromWho)
{
	monkProtectedUntil = until;
	monkProtectedBy = fromWho;
}

void Player::Kill(GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	isDead = true;
	GetCharacterOrDrunkBaseCharacter()->OnDeath(gameState, isExecutionKill, isDemonKill, sourcePlayer);
}

bool Player::HasGhostVote()
{
	return hasGhostVote;
}

void Player::UseGhostVote()
{
	hasGhostVote = false;
}

Character *Player::GetCharacterOrDrunkBaseCharacter()
{
	auto c = GetCharacter().get();
	if (auto d = dynamic_cast<Drunk*>(c))
		return d->DrunkBaseCharacter();
	else
		return c;
}

void Player::SwitchCharacter(CharacterType newType, GameState* gameState)
{
	character = CharacterHelper::CreateCharacter(newType, gameState, this, false);
	character->AnnounceCharacterAndAlignment(); // Sends their new character message
}

}
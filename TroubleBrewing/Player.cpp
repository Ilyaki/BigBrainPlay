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

Character* Player::GetCharacter()
{
	return character.get();
}

PlayerCommunication* Player::Communication()
{
	return communication.get();
}

bool Player::IsDead() const
{
	return isDead;
}

Player* Player::AttemptKill(GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	if (isDead)
	{
		// Hmm
		return nullptr;
	} else
	{
		auto [ allowDeath, redirectDeath, redirectTo ] =
				character->AllowCharacterDeath(gameState, isExecutionKill, isDemonKill, sourcePlayer);

		if (!allowDeath)
		{
			return nullptr;
		} else
		{
			if (redirectDeath)
			{
				assert(redirectTo != nullptr);
				return redirectTo->AttemptKill(gameState, isExecutionKill, isDemonKill, sourcePlayer);
			}
			else
			{
				bool monkProtected = (
								isDemonKill &&
								monkProtectedUntil >= gameState->GameStateGetCurrentTime() &&
								monkProtectedBy != nullptr &&
								!monkProtectedBy->IsDead() &&

								// If the Monk changes character they'd lose the ability (Can't actually happen in TB)
								poisonedBy->GetCharacter()->GetCharacterType() == CharacterType::MONK &&

								!gameState->AbilityMalfunctions(monkProtectedBy)
									  );

				if (monkProtected)
					return nullptr;
				else
				{
					Kill(gameState, isExecutionKill, isDemonKill, sourcePlayer);
					return this;
				}
			}
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
	auto currentTime = gameState->GameStateGetCurrentTime();
	return	poisonedUntil >= currentTime &&
			poisonedBy != nullptr &&
			!poisonedBy->IsDead() &&

			// If the original Imp suicides then the poisoner would lose the poison ability since they are now the Imp
			poisonedBy->GetCharacter()->GetCharacterType() == CharacterType::POISONER &&

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
	GetCharacterOrDrunkBaseCharacter()->PreDeath(gameState, isExecutionKill, isDemonKill, sourcePlayer);
	isDead = true;
	gameState->LogDeath(this, gameState->GameStateGetCurrentTime());
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
	auto c = GetCharacter();
	if (auto d = dynamic_cast<Drunk*>(c))
		return d->DrunkBaseCharacter();
	else
		return c;
}

void Player::SwitchCharacter(CharacterType newType, GameState* gameState)
{
	character = CharacterHelper::CreateCharacter(newType, gameState, this, false);
	character->AnnounceCharacterAndAlignment(gameState); // Sends their new character message
}

}
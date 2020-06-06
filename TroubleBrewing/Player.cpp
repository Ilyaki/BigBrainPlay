#include <iostream>
#include <cassert>
#include "Player.hpp"
#include "Characters/TestCharacter.hpp"
#include "Characters/Washerwoman.hpp"
#include "Characters/Librarian.hpp"
#include "Characters/Investigator.hpp"
#include "Characters/Chef.hpp"
#include "Characters/Empath.hpp"
#include "Characters/FortuneTeller.hpp"
#include "Characters/Undertaker.hpp"
#include "Characters/Monk.hpp"
#include "Characters/Ravenkeeper.hpp"

using namespace TroubleBrewing;

Player::Player(CharacterType characterType,
		PlayerData _playerData,
		std::shared_ptr<PlayerCommunication> _communication) :
	communication{_communication},
	playerData{_playerData}
{
	assert(_playerData.playerID >= 0);

	switch(characterType)
	{
		//TODO: cleanup somehow since all the constructors are similar?

		case CharacterType::TEST_CHARACTER:
			character = std::make_shared<TestCharacter>(this);
			break;

		case CharacterType::WASHERWOMAN:
			character = std::make_shared<Washerwoman>(this);
			break;

		case CharacterType::LIBRARIAN:
			character = std::make_shared<Librarian>(this);
			break;

		case CharacterType::INVESTIGATOR:
			character = std::make_shared<Investigator>(this);
			break;

		case CharacterType::CHEF:
			character = std::make_shared<Chef>(this);
			break;

		case CharacterType::EMPATH:
			character = std::make_shared<Empath>(this);
			break;

		case CharacterType::FORTUNETELLER:
			character = std::make_shared<FortuneTeller>(this);
			break;

		case CharacterType::UNDERTAKER:
			character = std::make_shared<Undertaker>(this);
			break;

		case CharacterType::MONK:
			character = std::make_shared<Monk>(this);
			break;

		case CharacterType::RAVENKEEPER:
			character = std::make_shared<Ravenkeeper>(this);
			break;

		case CharacterType::NO_CHARACTER:
		default:
			//TODO: Give error
			break;
	}

	assert(character != nullptr);
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

bool Player::AttemptKill(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer)
{
	assert((isExecutionKill != isDemonKill) && (isExecutionKill || isDemonKill));

	if (isDead)
	{
		Communication()->SendMessage("You have died. Again");
		return true;
	}
	else
	{
		if (character->AllowCharacterDeath(isExecutionKill, isDemonKill, sourcePlayer))
		{
			bool monkProtected = (monkProtectedUntil >= gameState->GetCurrentTime() &&
									monkProtectedBy != nullptr && !monkProtectedBy->AbilityMalfunctions(gameState));

			if (monkProtected)
				return false;
			else
			{
				Kill(gameState, isExecutionKill, isDemonKill, sourcePlayer);
				return true;
			}
		}
		else
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

bool Player::AbilityMalfunctions(GameState* gameState) const
{
	auto currentTime = gameState->GetCurrentTime();
	return !(poisonedUntil >= currentTime && poisonedBy != nullptr && !poisonedBy->AbilityMalfunctions(gameState));
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

void Player::Kill(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer)
{
	isDead = true;
	character->OnDeath(gameState, isExecutionKill, isDemonKill, sourcePlayer);
}

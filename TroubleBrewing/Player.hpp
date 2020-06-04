#ifndef BBP_CONSOLE_PLAYER_HPP
#define BBP_CONSOLE_PLAYER_HPP

#include <memory>
#include "Characters/Character.hpp"
#include "Characters/CharacterType.hpp"
#include "PlayerCommunication.hpp"
#include "PlayerData.hpp"

namespace TroubleBrewing
{
class Character;
class PlayerCommunication;

class Player
{
private:
	std::shared_ptr<PlayerCommunication> communication;
	PlayerData playerData;

	std::shared_ptr<Character> character;

	bool isDead {false};

	//TODO: add more detail, e.g. until when, etc..
	//list of effects, then calculate if drunk/poisoned? (maybe overkill if just the Poisoner)
	//maybe include other effects e.g. monk
	bool isPoisoned{false};

public:
	Player(CharacterType characterType, PlayerData _playerData, std::shared_ptr<PlayerCommunication> _communication);

	bool MatchesPlayerID(int playerID) const;
	std::string PlayerName() const;
	int PlayerID() const;

	bool IsDead() const;

	bool AbilityMalfunctions() const;

	/// AttemptKill: Tries to kill the player.
	/// \param isExecutionKill If the player is killed by execution (e.g. voting or Virgin nomination).
	/// \param isDemonKill If the player is killed by the demon.
	/// \return Returns true if the player is killed.
	bool AttemptKill(bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr);

	std::shared_ptr<Character> GetCharacter();
	std::shared_ptr<PlayerCommunication> Communication();

	// Moving
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	// Copying
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

};

}

#endif //BBP_CONSOLE_PLAYER_HPP

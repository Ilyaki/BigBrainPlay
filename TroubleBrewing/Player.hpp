#ifndef BBP_CONSOLE_PLAYER_HPP
#define BBP_CONSOLE_PLAYER_HPP

#include <memory>
#include "Characters/Character.hpp"
#include "Characters/CharacterType.hpp"
#include "PlayerCommunication.hpp"
#include "PlayerData.hpp"
#include "Time.hpp"

namespace TroubleBrewing
{
class Character;
class PlayerCommunication;

class Player
{
private:
	const std::shared_ptr<PlayerCommunication> communication{};
	PlayerData playerData;

	std::shared_ptr<Character> character{};

	bool isDead {false};
	bool hasGhostVote{true};

	// If adding more characters we may need an effects system (e.g. poisoned by poisoner, monk'd by monk, etc)
	// Would also be required if we had multiple characters of the same role
	Time poisonedUntil{};
	Player* poisonedBy{};

	Time monkProtectedUntil{};
	Player* monkProtectedBy{};

	void Kill(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer);

public:
	Player(CharacterType characterType, PlayerData _playerData,
			std::shared_ptr<PlayerCommunication> _communication, GameState* gameState);

	bool MatchesPlayerID(int playerID) const;
	std::string PlayerName() const;
	int PlayerID() const;

	bool IsDead() const;

	/// Should not be used directly other than in combination with the Character AbilityMalfunctions
	/// Checks if an ability should malfunction due to something on the player side (e.g. Poisoned effect)
	bool PlayerPartialCheckAbilityMalfunctions(GameState* gameState) const;

	bool HasGhostVote();
	void UseGhostVote();

	/// AttemptKill: Tries to kill the player.
	/// \param isExecutionKill If the player is killed by execution (e.g. voting or Virgin nomination).
	/// \param isDemonKill If the player is killed by the demon.
	/// \param sourcePlayer The killer, if applicable.
	/// \return Pointer to the player that was killed. nullptr if no one died
	Player* AttemptKill(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr);

	void SetPoisoned(Time until, Player* fromWho);
	void SetMonkProtection(Time until, Player* fromWho);

	void SwitchCharacter(CharacterType newType, GameState* gameState);

	Character* GetCharacter(); // TODO: replace uses with DrunkBase where needed
	Character* GetCharacterOrDrunkBaseCharacter();
	PlayerCommunication* Communication();

	// Moving
	Player(Player&&) = delete;
	Player& operator=(Player&&) = delete;

	// Copying
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

};

}

#endif //BBP_CONSOLE_PLAYER_HPP

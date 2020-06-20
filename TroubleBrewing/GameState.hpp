#ifndef BBP_CONSOLE_GAMESTATE_HPP
#define BBP_CONSOLE_GAMESTATE_HPP

#include <vector>
#include <functional>
#include "Time.hpp"
#include "Player.hpp"
#include "Characters/CharacterType.hpp"
#include "GameLogging.hpp"
#include "Characters/CharacterTraits.hpp"

namespace TroubleBrewing
{
// Forward declaration
class Player;

//GameState: Holds all players and character info, etc.
class GameState : public GameLogging
{
	// Player is on the free store so the pointer used in Player constructor is the same as inside the vector.
	std::vector<std::shared_ptr<Player>> players{};
	std::vector<Player*> playerCache{};
	bool cacheValid { true };

	Time currentTime{};

protected:
	void GameStateSetCurrentTime(Time newTime);

public:
	GameState();

	// The name GetCurrentTime conflicts with a Windows macro
	Time GameStateGetCurrentTime();

	const std::vector<Player*>& GetPlayers();

	int NumberOfPlayers();

	std::pair<Player*, Player*> GetNeighbours(Player* player, bool allowDead);

	bool AbilityMalfunctions(Player* player);

	bool PlayersAreNeighbours(Player* first, Player* second, bool allowDead = false);

	void AddPlayer(std::shared_ptr<Player> player);

	int GetNumPlayersAlive();
};

}

#endif //BBP_CONSOLE_GAMESTATE_HPP

#ifndef BBP_CONSOLE_GAMESTATE_HPP
#define BBP_CONSOLE_GAMESTATE_HPP

#include <vector>
#include <functional>
#include "Time.hpp"
#include "Player.hpp"
#include "Characters/CharacterType.hpp"
#include "GameLogging.hpp"

namespace TroubleBrewing
{
// Forward declaration
class Player;

//GameState: Holds all players and character info, etc.
class GameState : public GameLogging
{
	// Player is on the free store so the pointer used in Player constructor is the same as inside the vector.
	std::vector<std::shared_ptr<Player>> players{};

	Time currentTime;

protected:
	void SetCurrentTime(Time newTime);

public:
	GameState();

	Time GetCurrentTime();

	std::vector<Player*> GetPlayers();

	int NumberOfPlayers();

	std::vector<Player*> MatchingPlayers(std::function<bool(Player* player)> predicate);

	std::vector<Player*> GetPlayersMatchingCharacter(CharacterType characterType);
	std::vector<Player*> GetAlive();
	std::vector<Player*> GetEvil();
	std::vector<Player*> GetGood();
	std::vector<Player*> GetTownsfolk();
	std::vector<Player*> GetMinions();
	std::vector<Player*> GetOutsiders();
	std::vector<Player*> GetDemons();

	std::pair<Player*, Player*> GetNeighbours(Player* player, bool allowDead);

	bool PlayersAreNeighbours(Player* first, Player* second, bool allowDead = false);

	void AddPlayer(std::shared_ptr<Player> player);
};

}

#endif //BBP_CONSOLE_GAMESTATE_HPP

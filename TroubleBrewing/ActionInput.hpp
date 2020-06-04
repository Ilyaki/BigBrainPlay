#ifndef BBP_CONSOLE_ACTIONINPUT_HPP
#define BBP_CONSOLE_ACTIONINPUT_HPP

#include <functional>
#include "Player.hpp"
#include "GameState.hpp"

namespace TroubleBrewing
{
class Player;
class GameState;

// Provides interface for player to input another player
class ActionInput
{
public:
	using InputPlayerPredicate = std::pair<std::function<bool(Player* player)>, std::string>;
	virtual Player* InputPlayer(GameState* gameState, Player* sourcePlayer, std::string messageRequest, InputPlayerPredicate predicate = {nullptr, ""}) = 0;

	virtual ~ActionInput(){};
};

}

#endif //BBP_CONSOLE_ACTIONINPUT_HPP

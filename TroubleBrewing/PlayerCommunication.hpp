#ifndef BBP_CONSOLE_PLAYERCOMMUNICATION_HPP
#define BBP_CONSOLE_PLAYERCOMMUNICATION_HPP

#include <string>
#include "ActionInput.hpp"

namespace TroubleBrewing
{
class ActionInput;

/*class PlayerCommunication
{
	std::unique_ptr<ActionInput> actionInput;

public:
	explicit PlayerCommunication();

	ActionInput* GetActionInput();
	void SendMessage(std::string message);
};*/

class PlayerCommunication
{
public:
	using InputPlayerPredicate = std::pair<std::function<bool(Player* player)>, std::string>;
	virtual Player* InputPlayer(
			GameState* gameState,
			Player* sourcePlayer, std::string messageRequest,
			InputPlayerPredicate predicate = {nullptr, ""}
			) = 0;

	//TODO: Message types for formatting?
	virtual void SendMessage(std::string msg,  bool flush = true) = 0;
};

}

#endif //BBP_CONSOLE_PLAYERCOMMUNICATION_HPP

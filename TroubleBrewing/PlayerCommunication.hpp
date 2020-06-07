#ifndef BBP_CONSOLE_PLAYERCOMMUNICATION_HPP
#define BBP_CONSOLE_PLAYERCOMMUNICATION_HPP

#include <string>
#include "ActionInput.hpp"
#include "Voting.hpp"
#include "DayActions.hpp"

namespace TroubleBrewing
{
class ActionInput;

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

	virtual void OpenCloseNominations(
			bool open, GameState* gameState = nullptr, Voting* voting = nullptr, Player* sourcePlayer = nullptr) = 0;

	virtual void OpenCloseVoting(bool open, bool ghostVote = false,
			TroubleBrewing::Voting* voting = nullptr, Player* sourcePlayer = nullptr, int voteTimeSeconds = 0) = 0;

	virtual void OpenCloseDayActions(bool open, DayActions* dayActions = nullptr,
			GameState* gameState = nullptr, Player* sourcePlayer = nullptr) = 0;
};

}

#endif //BBP_CONSOLE_PLAYERCOMMUNICATION_HPP

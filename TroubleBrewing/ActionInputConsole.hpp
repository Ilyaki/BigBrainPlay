#ifndef BBP_CONSOLE_ACTIONINPUTCONSOLE_HPP
#define BBP_CONSOLE_ACTIONINPUTCONSOLE_HPP

#include "ActionInput.hpp"

namespace TroubleBrewing
{

class ActionInputConsole : public ActionInput
{
	Player* InputPlayer(GameState* gameState, Player* sourcePlayer, std::string messageRequest, ActionInput::InputPlayerPredicate predicate) override;
};

}

#endif //BBP_CONSOLE_ACTIONINPUTCONSOLE_HPP

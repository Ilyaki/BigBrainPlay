#ifndef BBP_GAMELOGGING_HPP
#define BBP_GAMELOGGING_HPP

#include <vector>

namespace TroubleBrewing
{
class Player;

class GameLogging
{
	std::vector<Player*> executionDeaths{};

protected:
	/// Sets the last player killed by exection. Set nullptr if no-one was killed by execution
	void SetLastExecutionDeath(Player* player);

public:
	/// \return Gets the last player to be killed by execution. Returns nullptr if no-one was killed by execution
	Player* GetLastExecutionDeath();

	/// \return true after the first day, even if no one was executed
	bool HasAnyExecutionPhaseTakenPlace();
};

}

#endif //BBP_GAMELOGGING_HPP

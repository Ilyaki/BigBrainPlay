#ifndef BBP_GAMELOGGING_HPP
#define BBP_GAMELOGGING_HPP

#include <vector>

namespace TroubleBrewing
{
class Player;

class GameLogging
{
	std::vector<Player*> executionDeaths;

protected:
	/// Sets the last player killed by exection. Set nullptr if no-one was killed by execution
	void SetLastExecutionDeath(Player* player);

public:
	/// \returns Gets the last player to be killed by execution. Returns nullptr if no-one was killed by execution
	Player* GetLastExecutionDeath();
};

}

#endif //BBP_GAMELOGGING_HPP

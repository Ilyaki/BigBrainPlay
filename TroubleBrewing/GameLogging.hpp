#ifndef BBP_GAMELOGGING_HPP
#define BBP_GAMELOGGING_HPP

#include <vector>
#include "Time.hpp"

namespace TroubleBrewing
{
class Player;

class GameLogging
{
	std::vector<std::pair<Player*, Time>> executionDeaths{};
	std::vector<std::pair<Player*, Time>> deaths{};
	std::vector<std::pair<Player*, Time>> nominees{};
	std::vector<std::pair<Player*, Time>> nominators{};

protected:
	/// Sets the last player killed by exection. Set nullptr if no-one was killed by execution
	void SetLastExecutionDeath(Player *player, Time executionTime);

	void AddNomination(Player* nominee, Player* nominator, Time time);

	bool HasBeenNominee(Player* nominee, Time time);
	bool HasBeenNominator(Player* nominator, Time time);

public:
	/// \return Gets the last player to be killed by execution. Returns nullptr if no-one was killed by execution
	std::pair<Player*, Time> GetLastExecutionDeath();

	const std::vector<std::pair<Player*, Time>>* GetDeaths();

	void LogDeath(Player* player, Time deathTime);

	/// \return true after the first day, even if no one was executed
	bool HasAnyExecutionPhaseTakenPlace();
};

}

#endif //BBP_GAMELOGGING_HPP

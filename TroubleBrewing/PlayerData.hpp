#ifndef BBP_CONSOLE_PLAYERDATA_HPP
#define BBP_CONSOLE_PLAYERDATA_HPP

#include <string>

namespace TroubleBrewing
{

class PlayerData
{
public:
	const std::string playerName;
	const int playerID;

	PlayerData(std::string _playerName, int _playerID) : playerName{_playerName}, playerID{_playerID} {};
};

}

#endif //BBP_CONSOLE_PLAYERDATA_HPP

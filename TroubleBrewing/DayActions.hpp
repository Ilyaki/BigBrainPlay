#ifndef BBP_DAYACTIONS_HPP
#define BBP_DAYACTIONS_HPP

namespace TroubleBrewing
{
class Player;

class DayActions
{
public:
	virtual void InformSlayerAttempt(Player* target, Player* sourcePlayer) = 0;
};

}

#endif //BBP_DAYACTIONS_HPP

#ifndef BBP_VOTING_HPP
#define BBP_VOTING_HPP

#include "Player.hpp"

namespace TroubleBrewing
{

// Interface implemented by Storyteller to make interface for PlayerCommunication to inform Storyteller of nominations
class Voting
{
	//TODO: move implementations into here instead of Storyteller?

public:
	virtual void InformNomination(Player* nominee, Player* nominator) = 0;
	virtual void InformVote(Player* sourcePlayer, bool vote) = 0;
};

}

#endif //BBP_VOTING_HPP

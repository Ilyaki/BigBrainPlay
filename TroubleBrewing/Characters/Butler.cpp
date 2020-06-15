#include "Butler.hpp"

namespace TroubleBrewing
{

void Butler::NightAction(bool zerothNight, GameState *gameState)
{
	master = player->Communication()->InputPlayer(gameState, player,
												  GetCharacterNameString() + ": Choose your Master",
												  {[this](Player *p) { return p != this->player; },
												   "You cannot choose yourself"});
}

bool Butler::CanButlerVote(const std::map<Player *, bool> &votes, GameState* gameState)
{
	if (player->IsDead() || gameState->AbilityMalfunctions(player))
	{
		// If dead, the Butler has no ability hence doesn't need Master
		return true;
	}
	else
	{
		for (auto &[player, voted] : votes)
		{
			if (player == master && voted)
				return true;
		}

		return false;
	}
}

}
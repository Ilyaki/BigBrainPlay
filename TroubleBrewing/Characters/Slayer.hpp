#ifndef BBP_SLAYER_HPP
#define BBP_SLAYER_HPP

#include "Character.hpp"
#include "../Player.hpp"

namespace TroubleBrewing
{

class Slayer : public Character
{
public:
	explicit Slayer(Player *_player) :
			Character(_player, "Slayer", CharacterType::SLAYER, CharacterTraits::Townsfolk()) {};

	/// \return true if the target was slain
	bool AttemptSlay(Player *target, GameState *gameState);
};

}


#endif //BBP_SLAYER_HPP

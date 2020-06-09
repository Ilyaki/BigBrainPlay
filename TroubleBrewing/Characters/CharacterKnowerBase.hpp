#ifndef BBP_CONSOLE_CHARACTERKNOWERBASE_HPP
#define BBP_CONSOLE_CHARACTERKNOWERBASE_HPP

#include <string>
#include "../GameState.hpp"
#include "Character.hpp"

namespace TroubleBrewing
{

// Washerwoman, Librarian and Investigator have almost identical abilities.
// Base class to not repeat code.
class CharacterKnowerBase : public Character
{
public:
	CharacterKnowerBase(Player* player) : Character(player){}

	constexpr virtual CharacterTraits TargetTraits() = 0;

	void NightAction(bool zerothNight, GameState* gameState) final;
};

}

#endif //BBP_CONSOLE_CHARACTERKNOWERBASE_HPP

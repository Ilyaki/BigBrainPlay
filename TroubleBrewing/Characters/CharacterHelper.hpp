#ifndef BBP_CHARACTERHELPER_HPP
#define BBP_CHARACTERHELPER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class CharacterHelper
{
public:
	static std::shared_ptr<Character> CreateCharacter(CharacterType characterType,
			GameState* gameState, Player* player, bool isDrunk);
};

}

#endif //BBP_CHARACTERHELPER_HPP

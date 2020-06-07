#ifndef BBP_MONK_HPP
#define BBP_MONK_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Monk : public Character
{
public:
	explicit Monk(Player* _player);

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_MONK_HPP

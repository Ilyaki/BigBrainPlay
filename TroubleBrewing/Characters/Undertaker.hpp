#ifndef BBP_UNDERTAKER_HPP
#define BBP_UNDERTAKER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Undertaker : public Character
{
public:
	explicit Undertaker(Player* _player);

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_UNDERTAKER_HPP

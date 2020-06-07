#ifndef BBP_CONSOLE_FORTUNETELLER_HPP
#define BBP_CONSOLE_FORTUNETELLER_HPP

#include "../Player.hpp"
#include "../GameState.hpp"

namespace TroubleBrewing
{

class FortuneTeller : public Character
{
	Player* redHerring;

public:
	explicit FortuneTeller(Player* _player);

	void InitialSetup(GameState* gameState) override;
	void NightAction(bool zerothNight, GameState* gameState) override;
};

}


#endif //BBP_CONSOLE_FORTUNETELLER_HPP

#ifndef BBP_CONSOLE_CHEF_HPP
#define BBP_CONSOLE_CHEF_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Chef : public Character
{
public:
	explicit Chef(Player* _player);

	void NightAction(bool zerothNight, GameState* gameState) override;

	std::string GetCharacterName() override;
	CharacterType GetCharacterType() override;
};

}

#endif //BBP_CONSOLE_CHEF_HPP
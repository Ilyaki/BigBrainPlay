#ifndef BBP_CONSOLE_EMPATH_HPP
#define BBP_CONSOLE_EMPATH_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Empath : public Character
{
public:
	explicit Empath(Player* _player);

	void NightAction(bool zerothNight, GameState* gameState) override;

	std::string GetCharacterName() override;
	CharacterType GetCharacterType() override;
};

}


#endif //BBP_CONSOLE_EMPATH_HPP

#ifndef BBP_CONSOLE_CHEF_HPP
#define BBP_CONSOLE_CHEF_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Chef : public Character
{
public:
	Chef(Player* player) : Character(player){}

	static constexpr CharacterType CharType = CharacterType::CHEF;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Chef";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_CONSOLE_CHEF_HPP
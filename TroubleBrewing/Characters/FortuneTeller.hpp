#ifndef BBP_CONSOLE_FORTUNETELLER_HPP
#define BBP_CONSOLE_FORTUNETELLER_HPP

#include "../Player.hpp"
#include "../GameState.hpp"

namespace TroubleBrewing
{

class FortuneTeller : public Character
{
	Player* redHerring{};

public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::FORTUNETELLER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Fortune Teller";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void InitialSetup(GameState* gameState) override;
	void NightAction(bool zerothNight, GameState* gameState) override;
};

}


#endif //BBP_CONSOLE_FORTUNETELLER_HPP

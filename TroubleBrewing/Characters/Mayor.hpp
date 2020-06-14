#ifndef BBP_MAYOR_HPP
#define BBP_MAYOR_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Mayor: public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::MAYOR;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Outsider();
	static constexpr std::string_view CharName  = "Mayor";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	std::tuple<bool, bool, Player*> AllowCharacterDeath(
			GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr) override;
};

}

#endif //BBP_MAYOR_HPP

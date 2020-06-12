#ifndef BBP_RAVENKEEPER_HPP
#define BBP_RAVENKEEPER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Ravenkeeper : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::RAVENKEEPER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Ravenkeeper";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void OnDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer) override;
};

}

#endif //BBP_RAVENKEEPER_HPP

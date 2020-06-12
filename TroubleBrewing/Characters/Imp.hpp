#ifndef BBP_IMP_HPP
#define BBP_IMP_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Imp : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::IMP;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Demon();
	static constexpr std::string_view CharName  = "Imp";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void NightAction(bool zerothNight, GameState* gameState) override;

	void OnDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer) override;
};

}

#endif //BBP_IMP_HPP

#ifndef BBP_BUTLER_HPP
#define BBP_BUTLER_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Butler : public Character
{
	Player* master { nullptr };

public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::BUTLER;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Outsider();
	static constexpr std::string_view CharName  = "Butler";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	bool CanButlerVote(const std::map<Player *, bool>& votes, GameState* gameState);

	virtual void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_BUTLER_HPP

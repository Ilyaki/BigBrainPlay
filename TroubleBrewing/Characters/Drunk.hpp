#ifndef BBP_DRUNK_HPP
#define BBP_DRUNK_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Drunk: public Character
{
	CharacterType perceivedCharacterType;
	std::shared_ptr<Character> perceivedCharacter;

public:
	Drunk(Player *player, GameState* gameState, Time creationTime);

	static constexpr CharacterType CharType = CharacterType::DRUNK;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Outsider();
	static constexpr std::string_view CharName  = "Drunk";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	CharacterType GetPerceivedCharacter() override;

	Character* DrunkBaseCharacter();

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_DRUNK_HPP

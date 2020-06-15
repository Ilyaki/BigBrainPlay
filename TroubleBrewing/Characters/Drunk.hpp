#ifndef BBP_DRUNK_HPP
#define BBP_DRUNK_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Drunk: public Character
{
	CharacterType perceivedCharacterType{};
	std::shared_ptr<Character> perceivedCharacter{};

public:
	Drunk(Player *player, GameState* gameState, Time _creationTime) : Character(player, true, _creationTime){};

	void InitialSetup(GameState* gameState) override;

	static constexpr CharacterType CharType = CharacterType::DRUNK;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Outsider();
	static constexpr std::string_view CharName  = "Drunk";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	void PreDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer) override;

	PerceivedCharacterData GeneratePerceivedCharacterData(GameState* gameState) override;

	Character* DrunkBaseCharacter();

	bool AbilityWorksWhenDead() override;

	void NightAction(bool zerothNight, GameState* gameState) override;
};

}

#endif //BBP_DRUNK_HPP

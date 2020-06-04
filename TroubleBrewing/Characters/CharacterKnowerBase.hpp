#ifndef BBP_CONSOLE_CHARACTERKNOWERBASE_HPP
#define BBP_CONSOLE_CHARACTERKNOWERBASE_HPP

#include <string>
#include "../GameState.hpp"
#include "Character.hpp"

namespace TroubleBrewing
{

// Washerwoman, Librarian and Investigator have almost identical abilities.
// Base class to not repeat code.
class CharacterKnowerBase : public Character
{
	Player* firstPlayer;
	Player* secondPlayer;
	CharacterType knownCharacter{};

public:
	explicit CharacterKnowerBase(Player* _player, CharacterTraits traits = CharacterTraits::Townsfolk());

	constexpr virtual CharacterTraits TargetTraits() = 0;

	void InitialSetup(GameState* gameState) final;
	void NightAction(bool zerothNight, GameState* gameState) final;

	std::string GetCharacterName() override = 0;
	CharacterType GetCharacterType() override = 0;
};

}

#endif //BBP_CONSOLE_CHARACTERKNOWERBASE_HPP

#ifndef BBP_CONSOLE_CHARACTER_HPP
#define BBP_CONSOLE_CHARACTER_HPP

#include <string>
#include "CharacterType.hpp"
#include "CharacterTraits.hpp"
#include "../GameState.hpp"
#include "../Player.hpp"

namespace TroubleBrewing
{
class GameState;
class Player;

class Character
{
protected:
	Player* player;

	Character(Player* _player) : player{_player}{}

public:
	inline std::string GetCharacterNameString() const { return std::string { GetCharacterName() }; }
	constexpr virtual std::string_view GetCharacterName() const = 0;
	constexpr virtual CharacterType GetCharacterType() = 0;
	[[deprecated]] constexpr virtual CharacterTraits GetCharacterTraits() = 0;

	/// Characters like Recluse & Spy can register as different characters.
	virtual CharacterTraits GeneratePerceivedTraits();

	/// AllowCharacterDeath: Returns false if a death should be prevented, (e.g. Soldier), otherwise false.
	virtual bool AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr);

	/// OnDeath: Call when the player of this character has died
	virtual void OnDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer){};

	virtual void InitialSetup(GameState* gameState);

	virtual void NightAction(bool zerothNight, GameState* gameState){};

	virtual ~Character(){};
};

#define TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS constexpr std::string_view GetCharacterName() const override { return CharName; }; \
constexpr CharacterType GetCharacterType() override { return CharType; }; \
constexpr CharacterTraits GetCharacterTraits() override { return CharTraits; }

}

#endif //BBP_CONSOLE_CHARACTER_HPP

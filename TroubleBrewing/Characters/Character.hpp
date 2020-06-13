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
	/// If we are the Drunk character
	const bool isDrunk { false };
	const Time creationTime{};

protected:
	Player* player{};

public:
	Character(Player* _player, bool _isDrunk, Time _creationTime) :
		isDrunk{_isDrunk}, creationTime{_creationTime}, player{_player} {}

	inline std::string GetCharacterNameString() const { return std::string { GetCharacterName() }; }
	constexpr virtual std::string_view GetCharacterName() const = 0;
	[[deprecated]] constexpr virtual CharacterType GetCharacterType() = 0;
	[[deprecated]] constexpr virtual CharacterTraits GetCharacterTraits() = 0;

	void AnnounceCharacterAndAlignment();

	virtual bool AbilityWorksWhenDead() { return false; }

	Time GetCreationTime() const;

	/// Characters like Recluse & Spy can register as different characters.
	virtual CharacterTraits GeneratePerceivedTraits();

	/// The Drunk thinks they are another character
	virtual CharacterType GetPerceivedCharacter(GameState* gameState);

	/// AllowCharacterDeath: Returns false if a death should be prevented, (e.g. Soldier), otherwise false.
	virtual bool AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr);

	/// OnDeath: Call when the player of this character has died
	virtual void OnDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer){};

	virtual void InitialSetup(GameState* gameState){};

	/// \param isDrunk Provided so to know when to not apply an effect (e.g. Monk protection) and when to (if poisoned)
	virtual void NightAction(bool zerothNight, GameState* gameState){};

	/// Should not be used directly other than in combination with the Player AbilityMalfunctions
	/// Checks if this character is the Drunk
	bool IsDrunk() const;

	virtual ~Character(){};
};

#define TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS constexpr std::string_view GetCharacterName() const override { return CharName; }; \
constexpr CharacterType GetCharacterType() override { return CharType; }; \
constexpr CharacterTraits GetCharacterTraits() override { return CharTraits; }

}

#endif //BBP_CONSOLE_CHARACTER_HPP

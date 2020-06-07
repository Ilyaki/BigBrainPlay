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
public:
    Player* player;
	const CharacterTraits traits;

	Character(Player* _player, CharacterTraits _traits) : player{_player}, traits{_traits} {};

	//TODO: these could be moved into the Character constructor
	virtual std::string GetCharacterName() = 0;
	virtual CharacterType GetCharacterType() = 0;

	/// AllowCharacterDeath: Returns false if a death should be prevented, (e.g. Soldier), otherwise false.
	virtual bool AllowCharacterDeath(bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr);

	/// OnDeath: Call when the player of this character has died
	virtual void OnDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer){};

	virtual void InitialSetup(GameState* gameState);

	virtual void NightAction(bool zerothNight, GameState* gameState){};

	virtual ~Character(){};
};

}

#endif //BBP_CONSOLE_CHARACTER_HPP

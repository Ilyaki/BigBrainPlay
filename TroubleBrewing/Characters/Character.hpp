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
	const std::string characterName;
	const CharacterType characterType;

protected:
	Character(Player* _player, std::string _characterName, CharacterType _characterType, CharacterTraits _traits) :
			player{_player}, traits{_traits}, characterName{_characterName}, characterType{_characterType} {};

    Player* player;

public:
	const CharacterTraits traits;// TODO: make getter

	std::string GetCharacterName() const;
	CharacterType GetCharacterType() const;

	/// AllowCharacterDeath: Returns false if a death should be prevented, (e.g. Soldier), otherwise false.
	virtual bool AllowCharacterDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer = nullptr);

	/// OnDeath: Call when the player of this character has died
	virtual void OnDeath(GameState* gameState, bool isExecutionKill, bool isDemonKill, Player* sourcePlayer){};

	virtual void InitialSetup(GameState* gameState);

	virtual void NightAction(bool zerothNight, GameState* gameState){};

	virtual ~Character(){};
};

}

#endif //BBP_CONSOLE_CHARACTER_HPP

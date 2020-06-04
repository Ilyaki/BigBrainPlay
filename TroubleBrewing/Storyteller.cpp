#include "Storyteller.hpp"
#include "Player.hpp"
#include "Characters/TestCharacter.hpp"
#include <iostream>
#include <cassert>


using namespace TroubleBrewing;

Storyteller::Storyteller(std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>> playerDatas) : GameState{}
{
	// Will need to change charsInPlay dynamically in future.
	assert(playerDatas.size() == charactersInPlay.size());

	// Add characters
	/*int playerID = 1;
	for (CharacterType charInPlay : charactersInPlay)
	{
		//TODO: needs to be filled with player ID/name, etc.
		std::string playerName = "Player";
		playerName += std::to_string(playerID);
		AddPlayer(std::make_shared<Player>(charInPlay, playerName, playerID));
		++playerID;
	}*/
	int playerI = 0;
	for (auto pd : playerDatas)
	{
		auto charInPlay = charactersInPlay.at(playerI);
		AddPlayer(std::make_shared<Player>(charInPlay, pd.first, pd.second));
		++playerI;
	}

	// Perform initial player setup
	for (Player* player : GetPlayers())
	{
		player->GetCharacter()->InitialSetup(this);
	}
}

//TODO: Take player ids/etc as arguments.
//TODO: Should be inside constructor to enforce player vector not empty invariant
void Storyteller::StartGame()
{
	std::cout << "Storyteller start game" << std::endl;

	// Zeroth night
	NightPhase(zerothNightOrder, 0);
}

void Storyteller::NightPhase(const std::vector<CharacterType> order, int night)
{
	const bool zerothNight = night == 0;

	std::cout << "Storyteller NightPhase, night = " << night << std::endl;

	for (CharacterType targetChar : order)
	{
		//TODO: Search for the Drunk by the character they think they are. (or use 'perceived character/etc')
		auto targetMatches = GetPlayersMatchingCharacter(targetChar);
		for (Player* target : targetMatches)
			//TODO: Log the action, test win conditions?
			target->GetCharacter()->NightAction(zerothNight, this);
	}
}

bool Storyteller::GameFinished()
{
	//TODO: Storyteller::GameFinished
	return false;
}

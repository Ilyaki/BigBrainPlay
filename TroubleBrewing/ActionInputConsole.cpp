#include <iostream>
#include <algorithm>
#include "ActionInputConsole.hpp"

namespace TroubleBrewing
{

Player *ActionInputConsole::InputPlayer(GameState *gameState, Player *sourcePlayer, std::string messageRequest, InputPlayerPredicate predicate)
{
	//Print out player IDs
	std::cout << "Player IDs:\n";
	for (Player* player : gameState->GetPlayers())
	{
		std::cout << player->PlayerName() << ": " << player->PlayerID() << "\n";
	}
	std::cout << std::flush;


	std::string playerName {sourcePlayer->PlayerName()};
	std::cout << "[" << playerName << "] " << messageRequest << std::endl;

	Player* foundTarget {nullptr};
	auto players = gameState->GetPlayers();

	while (foundTarget == nullptr)
	{
		std::cout << "Enter a player ID: ";

		int playerID;
		while (((std::cin >> playerID), (std::cin.fail())))
		{
			std::cout << "\nInvalid ID. Enter a player ID: ";
		}

		std::cout << "\nReceived input player ID = " << playerID << std::endl;


		auto index = std::find_if(players.begin(), players.end(),
								  [playerID](Player *p) { return p->MatchesPlayerID(playerID); });

		if (index == players.end()) // No player with player ID
		{
			std::cout << "No matching player found" << std::endl;
			continue;
		}
		else if (predicate.first && !predicate.first(*index)) // Doesn't match condition
		{
			std::cout << "Player doesn't match conditions: " << predicate.second << std::endl;
			continue;
		}
		else // Found target
		{
			foundTarget = *index;
			break;
		}
	}

	return foundTarget;
}

}
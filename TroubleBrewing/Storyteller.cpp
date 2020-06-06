#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "Storyteller.hpp"
#include "Player.hpp"
#include "Characters/TestCharacter.hpp"

using namespace TroubleBrewing;

Storyteller::Storyteller(std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>> playerDatas) : GameState{}
{
	// Will need to change charsInPlay dynamically in future.
	assert(playerDatas.size() == charactersInPlay.size());

	// Add players
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
	//TODO: make a function to clear chat?
	// _ _ is invisible in Discord
	AnnounceMessage("_ _\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n"
		"\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n"
	 	"\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n_ _\\n"
   		"Game has begun...");//TODO: Fill in start message.

	// Zeroth night
	SetCurrentTime(Time{false, 0});
	NightPhase(zerothNightOrder, 0);

	//TODO: win conditions
	for (int dayNight = 1;; dayNight++)
	{
		SetCurrentTime(Time{true, dayNight});
		DayPhase(dayNight);
		AnnounceMessage("_ _\\n\\n_ _", false);

		SetCurrentTime(Time{false, dayNight});
		NightPhase(nightOrder, dayNight);
		AnnounceMessage("_ _\\n\\n_ _", false);
	}
}

void Storyteller::NightPhase(const std::vector<CharacterType> order, int night)
{
	assert(night >= 0);

	const bool zerothNight = night == 0;

	AnnounceMessage("Night phase " + std::to_string(night) + " has begun. Everyone go to sleep");

	for (CharacterType targetChar : order)
	{
		//TODO: Search for the Drunk by the character they think they are. (or use 'perceived character/etc')
		//TODO: if not dead
		auto targetMatches = GetPlayersMatchingCharacter(targetChar);
		for (Player* target : targetMatches)
			//TODO: Log the action, test win conditions?
			target->GetCharacter()->NightAction(zerothNight, this);
	}
}

void Storyteller::DayPhase(int day)
{
	assert(day > 0);

	//TODO: announce who died, etc.
	AnnounceMessage("Day " + std::to_string(day) + " has begun. Everyone wake up");

	std::this_thread::sleep_for(std::chrono::seconds(3));

	// Open nominations
	choppingBlock = nullptr;
	choppingBlockVotes = 0;

	int minMajority = (GetAlive().size()/2) + 1;

	AnnounceMessage("Day " + std::to_string(day) + " nominations are now open. " +
		std::to_string(minMajority) + " votes are required for a majority");

	while(true)
	{
		if (!nominationsOpen) // Prevents message spam
			OpenCloseNominations(true);

		AnnounceMessage("Nominations closing in 5 seconds...");

		std::unique_lock<std::mutex> lock(informNominationCondMutex);
		if (informNominationCond.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::no_timeout)
		{
			// Must have a nomination
			ProcessNomination(std::get<0>(informNominationData), std::get<1>(informNominationData));
		}
		else
		{
			// Timeout. End nomination phase
			break;
		}
	}

	AnnounceMessage("Nominations closing!");

	OpenCloseNominations(false);

	ExecuteChoppingBlock();
}

void Storyteller::ExecuteChoppingBlock()
{
	if (choppingBlock == nullptr)
	{
		AnnounceMessage("Nobody was executed");
		SetLastExecutionDeath(nullptr);
	} else {
		bool wasKilled = choppingBlock->AttemptKill(this, true, false);

		if (wasKilled)
		{
			//TODO: spectacular execution messages?
			AnnounceMessage(choppingBlock->PlayerName() + " was executed and killed");
			SetLastExecutionDeath(choppingBlock);
		} else {
			AnnounceMessage(choppingBlock->PlayerName() + " was executed but did not die");
			SetLastExecutionDeath(nullptr);
		}
	}
}

void Storyteller::AnnounceMessage(std::string message, bool flush)
{
	for (Player* player : GetPlayers())
	{
		player->Communication()->SendMessage(message, flush);
	}
}

void Storyteller::InformNomination(Player *nominee, Player *nominator)
{
	// // Lock the mutex until InformNomination is returned from (ie this nomination and voting finished)
	//const std::lock_guard<std::mutex> lock(nominationOrVotingMutex);

	if (!nominationsOpen)
	{
		nominator->Communication()->SendMessage("Nominations are not currently open");
	} else{
		informNominationData = std::make_tuple(nominee, nominator);
		informNominationCond.notify_one();
	}
}

void Storyteller::ProcessNomination(Player *nominee, Player* nominator)
{
	//TODO: only allow 1 nomination & nominee from each player. Player must be alive.

	OpenCloseNominations(false);
	AnnounceMessage(nominee->PlayerName() + " has been nominated by " + nominator->PlayerName());

	// TODO: allow time for players to defend themselves

	constexpr int voteTimeSeconds = 5;
	OpenCloseVoting(true, voteTimeSeconds);

	//FIXME: this blocks the onMessage stuff
	std::this_thread::sleep_for(std::chrono::seconds(voteTimeSeconds));

	OpenCloseVoting(false);

	ManageVotes(votes, nominee, nominator);

	OpenCloseNominations(true);
}

void Storyteller::InformVote(Player *sourcePlayer, bool vote)
{
	//assert(has ghost vote)
	votes.insert(std::pair(sourcePlayer, vote));
	//TODO: deduct ghost vote if necessary
}

void Storyteller::OpenCloseNominations(bool open)
{
	nominationsOpen = open;

	for (Player* player : GetPlayers())
	{
		if (open)
			player->Communication()->OpenCloseNominations(true, this, this, player);
		else
			player->Communication()->OpenCloseNominations(false);
	}
}

bool Storyteller::ManageVotes(std::map<Player *, bool> votes, Player *nominee, Player* nominator)
{
	// Announce votes
	std::string votesMsg = R"(```yaml\nVotes to execute )" + nominee->PlayerName() + ":\\n";
	for (auto pair : votes)
	{
		votesMsg += "- " + pair.first->PlayerName() + ": " + (pair.second ? "yes" : "no");

		if (pair.first->IsDead())
			votesMsg += ". Used ghost vote";

		votesMsg += "\\n";
	}
	votesMsg += R"(```)";

	AnnounceMessage(votesMsg);

	// Count votes
	int numAlive = GetAlive().size();
	int numVotes = 0;
	for (auto pair : votes)
	{
		if (pair.second)
			++numVotes;
	}

	// Test majority
	if (numVotes > (numAlive / 2))
	{
		if (numVotes > choppingBlockVotes)
		{
			// Enough votes
			AnnounceMessage(
					nominee->PlayerName() + " is on the chopping block with " + std::to_string(numVotes) + " votes");

			choppingBlockVotes = numVotes;
			choppingBlock = nominee;
			return true;
		}
		else if (numVotes == choppingBlockVotes)
		{
			// Tie
			if(choppingBlock != nullptr)
			{
				AnnounceMessage(std::to_string(numVotes) + " votes for " + nominee->PlayerName() + " are tied with " +
								std::to_string(numVotes) + " votes for " + choppingBlock->PlayerName() +
								". Nobody is on the chopping block");
			}
			else
			{
				// Rare case: number of votes matched an existing tie
				AnnounceMessage(std::to_string(numVotes) + " votes for " + nominee->PlayerName() +
					" ties with the existing tie. Nobody is on the chopping block");
			}

			choppingBlock = nullptr;
			return false;
		}
		else
		{
			// Not enough votes
			AnnounceMessage(std::to_string(numVotes) + " votes for " + nominee->PlayerName() + " is less than " +
							std::to_string(choppingBlockVotes) + " votes for " + choppingBlock->PlayerName() +
							". " + choppingBlock->PlayerName() + " remains on the chopping block");
			return false;
		}
	} else {
		AnnounceMessage(std::to_string(numVotes) + " votes is not enough votes to execute " + nominee->PlayerName());
		return false;
	}
}

void Storyteller::OpenCloseVoting(bool open, int voteTimeSeconds)
{
	if (open)
	{
		votes.clear();

		for (Player *player : GetPlayers())
		{
			//TODO: test if player has ghost vote.
			if (!player->IsDead())
				player->Communication()->OpenCloseVoting(true, false, this, player, voteTimeSeconds);
		}
	}
	else
	{
		for (Player *player : GetPlayers())
		{
			if (!player->IsDead())
				player->Communication()->OpenCloseVoting(false);
		}
	}
}

bool Storyteller::GameFinished() const
{
	//TODO: Storyteller::GameFinished
	return false;
}

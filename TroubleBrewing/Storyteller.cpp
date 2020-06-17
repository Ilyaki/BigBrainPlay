#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include "Storyteller.hpp"
#include "Player.hpp"
#include "Characters/Virgin.hpp"
#include "Characters/Slayer.hpp"
#include "Characters/Butler.hpp"
#include "Characters/CharacterChooser.hpp"

namespace TroubleBrewing
{

Storyteller::Storyteller(const std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>>& playerDatas)
		: GameState{}
{
	//TODO: print expected count

	auto [ success, charactersInPlay, expectedCount ] = CharacterChooser::GenerateCharacterTypes(playerDatas);
	if (!success)
		throw std::out_of_range{ "Too many or too few players" };

	int playerI = 0;
	for (auto pd : playerDatas)
	{
		auto charInPlay = charactersInPlay.at(playerI);
		AddPlayer(std::make_shared<Player>(charInPlay, pd.first, pd.second, this));
		++playerI;
	}

	// Perform initial player setup
	for (Player *player : GetPlayers())
	{
		player->Communication()->BlankPage();
		player->Communication()->SendMessage("Game has begun...");
		player->GetCharacter()->InitialSetup(this);
		player->Communication()->PrintPlayerIDs(this);
		player->GetCharacter()->AnnounceCharacterAndAlignment(this, true, GetPlayers().size() <= 6);
	}
}

void Storyteller::StartGame()
{
	// Zeroth night
	SetCurrentTime(Time{false, 0});
	NightPhase(zerothNightOrder, 0);

	for (int dayNight = 1;; dayNight++)
	{
		const auto& players = GetPlayers();

		SetCurrentTime(Time{true, dayNight});
		DayPhase(dayNight);
		std::for_each(players.begin(), players.end(), [](Player* p){ p->Communication()->NewParagraph(); });
		if (auto [gameEnded, evilWin, winType] = CheckGameWin(false); gameEnded)
		{
			ManageWin(evilWin, winType);
			break;
		}

		SetCurrentTime(Time{false, dayNight});
		NightPhase(nightOrder, dayNight);
		std::for_each(players.begin(), players.end(), [](Player* p){ p->Communication()->NewParagraph(); });
		if (auto [gameEnded, evilWin, winType] = CheckGameWin(false); gameEnded)
		{
			ManageWin(evilWin, winType);
			break;
		}
	}
}

void Storyteller::NightPhase(const std::vector<CharacterType> order, int night)
{
	assert(night >= 0);

	const bool zerothNight = night == 0;

	AnnounceMessage("Night phase " + std::to_string(night) + " has begun. Everyone go to sleep");

	std::vector<Player*> playerActionOrder;
	for (CharacterType targetChar : order)
	{
		for (auto target : GetPlayers())
		{
			// Don't use a sorting algorithm in case GenPerceived is called multiple times
			// (Not a problem with Drunk, maybe with future roles)
			if (target->GetCharacter()->GeneratePerceivedCharacterData(this).selfPerceivedCharacterType == targetChar &&
				(target->GetCharacter()->AbilityWorksWhenDead() || !target->IsDead()) )
				playerActionOrder.push_back(target);
		}
	}

	for(Player* player : playerActionOrder)
	{
		auto character = player->GetCharacter();

		// Don't let a new character perform a night action (unless its the zeroth night)
		auto creationTime = character->GetCreationTime();
		auto currentTime = GetCurrentTime();

		if (creationTime != currentTime || currentTime == Time { false, 0 })
			character->NightAction(zerothNight, this);
	}

	//TODO: Add wait here so players don't know if anyone came after them
}

void Storyteller::AnnounceNightDeaths()
{
	Time lastNight = Time {false, GetCurrentTime().DayOrNightCount() - 1};
	std::vector<Player *> deathsLastNight {};
	for (auto &[player, deathTime] : *GetDeaths())
	{
		if (deathTime == lastNight)
			deathsLastNight.push_back(player);
	}

	const auto deathsLastNightSize = deathsLastNight.size();

	if (deathsLastNightSize == 0)
		AnnounceMessage("There were no deaths last night", false);
	else
	{
		// Gives a message like "Alpha, Beta and Gamma"
		std::string msg{};
		long unsigned int i = 0;
		for (Player* p : deathsLastNight)
		{
			if (i == 0)
				msg += p->PlayerName();
			else if (i == deathsLastNightSize - 1)
				msg += " and " + p->PlayerName();
			else
				msg += ", " + p->PlayerName();

			++i;
		}

		AnnounceMessage(msg + " died last night", false);
	}
}

void Storyteller::DayPhase(int day)
{
	assert(day > 0);

	AnnounceNightDeaths();

	constexpr auto dayTimeLengthSeconds = 20;
	const auto dayTimeEnd = std::chrono::steady_clock::now() + std::chrono::seconds(dayTimeLengthSeconds);

	AnnounceMessage("Day " + std::to_string(day) + " has begun, ending in " + std::to_string(dayTimeLengthSeconds) +
					" seconds. Everyone wake up");

	OpenCloseDayActions(true);

	// Day action wait loop
	std::unique_lock<std::mutex> dayTimeLock(slayActionCondMutex);
	bool wasNotified;
	while (
				(slayActionCondWaiting = true),
				(wasNotified = slayActionCond.wait_until(dayTimeLock, dayTimeEnd,
						[this](){ return !slayActionCondWaiting; })),
				(slayActionCondWaiting = false),
				(wasNotified)
				)
	{
		// Have a slay action (not a timeout)
		Player* playerKilled =
				ProcessSlayAction(std::get<0>(slayActionData), std::get<1>(slayActionData));

		if (playerKilled && std::get<0>(CheckGameWin(false)))
		{
			return;
		}
	}
	// Received a timeout, so the day action phase is over.

	OpenCloseDayActions(false);

	// Open nominations
	choppingBlock = nullptr;
	choppingBlockVotes = 0;

	const auto numAlive = GetNumPlayersAlive();
	int minMajority = (numAlive / 2) + (numAlive % 2);

	AnnounceMessage("Day " + std::to_string(day) + " nominations are now open. " +
					std::to_string(minMajority) + " votes are required for a majority");

	while (true)
	{
		if (!nominationsOpen) // Prevents message spam
			OpenCloseNominations(true);

		constexpr int nominationSeconds = 15;
		AnnounceMessage("Nominations closing in " + std::to_string(nominationSeconds) + " seconds...");

		std::unique_lock<std::mutex> lock(informNominationCondMutex);
		informNominationCondWaiting = true;
		bool wasNotified = informNominationCond.wait_for(lock, std::chrono::seconds(nominationSeconds),
				[this](){ return !informNominationCondWaiting; });
		informNominationCondWaiting = false;

		if (wasNotified) // Not a timeout
		{
			// Must have a nomination

			bool abruptEnd = ProcessNomination(std::get<0>(informNominationData), std::get<1>(informNominationData));
			if (abruptEnd)
				break;
		} else
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
		SetLastExecutionDeath(nullptr, GetCurrentTime());
	} else
	{
		Player* whoDied = choppingBlock->AttemptKill(this, true, false);

		if (whoDied != nullptr)
		{
			//POLISH: spectacular execution messages?
			AnnounceMessage(whoDied->PlayerName() + " was executed and killed");
			SetLastExecutionDeath(whoDied, GetCurrentTime());
		} else
		{
			AnnounceMessage(choppingBlock->PlayerName() + " was executed but did not die");
			SetLastExecutionDeath(nullptr, GetCurrentTime());
		}
	}
}

void Storyteller::AnnounceMessage(const std::string& message, bool flush)
{
	for (Player *player : GetPlayers())
		player->Communication()->SendMessage(message, flush);
}

void Storyteller::InformNomination(Player *nominee, Player *nominator)
{
	if (!nominationsOpen)
	{
		nominator->Communication()->SendMessage("Nominations are not currently open");
	} else
	{
		informNominationData = std::make_tuple(nominee, nominator);
		informNominationCondWaiting = false;
		informNominationCond.notify_one();
	}
}

bool Storyteller::ProcessNomination(Player *nominee, Player *nominator)
{
	// Only allow 1 nomination & nominee from each player each day. Nominator player must be alive.
	if(nominator->IsDead())
	{
		AnnounceMessage(nominator->PlayerName() + " tried to nominate " + nominee->PlayerName() + ", but "
			+ nominator->PlayerName() + " is dead");
		return false;
	}
	else if (HasBeenNominator(nominator, GetCurrentTime()))
	{
		AnnounceMessage(nominator->PlayerName() + " tried to nominate " + nominee->PlayerName() + ", but "
						+ nominator->PlayerName() + " has already nominated someone today");
		return false;
	}
	else if (HasBeenNominee(nominee, GetCurrentTime()))
	{
		AnnounceMessage(nominator->PlayerName() + " tried to nominate " + nominee->PlayerName() + ", but "
						+ nominee->PlayerName() + " has already been nominated today");
		return false;
	}
	else
	{
		// Allow the nomination to pass
		AddNomination(nominee, nominator, GetCurrentTime());
	}


	OpenCloseNominations(false);
	AnnounceMessage(nominee->PlayerName() + " has been nominated by " + nominator->PlayerName());

	if (auto v = dynamic_cast<Virgin *>(nominee->GetCharacterOrDrunkBaseCharacter()))
	{
		if (v->VirginAbility(nominator, this)) // Should nominator be killed
		{
			// Virgin ability should execute nominator immediately.
			// Only one execution in a day, so move to next day immediately.
			choppingBlock = nominator;
			choppingBlockVotes = 0;
			return true;
		}
	}

	//FIXME: sleep_for returns immediately, this is a workaround
	//std::this_thread::sleep_for(std::chrono::seconds());
	auto sleepTime = [](int seconds)
	{
		std::condition_variable cv{};
		std::mutex cvm{};
		auto cvl = std::unique_lock<std::mutex>{cvm};
		cv.wait_for(cvl, std::chrono::seconds(seconds));
	};

	constexpr int explainNominationTimeSeconds = 15;
	constexpr int defendTimeSeconds = 15;

	AnnounceMessage(nominator->PlayerName() + " has " + std::to_string(explainNominationTimeSeconds) +
		" seconds to explain why they nominated " + nominee->PlayerName() + ". Everyone else be silent");

	sleepTime(explainNominationTimeSeconds);

	AnnounceMessage(nominee->PlayerName() + " has " + std::to_string(defendTimeSeconds) +
					" seconds to defend themself. Everyone else be silent");

	sleepTime(defendTimeSeconds);

	constexpr int voteTimeSeconds = 20;
	OpenCloseVoting(true, voteTimeSeconds);

	sleepTime(voteTimeSeconds);

	OpenCloseVoting(false);

	ManageVotes(votes, nominee, nominator);

	OpenCloseNominations(true);

	return false;
}

void Storyteller::InformVote(Player *sourcePlayer, bool vote)
{
	if (sourcePlayer->IsDead() && !sourcePlayer->HasGhostVote())
	{
		std::cerr << "Should not prompt dead player to vote when they have used their ghost vote" << std::endl;
		return;
	}
	else
	{
		votes.insert(std::pair(sourcePlayer, vote));
		if (sourcePlayer->IsDead())
			sourcePlayer->UseGhostVote();
	}
}

void Storyteller::OpenCloseNominations(bool open)
{
	nominationsOpen = open;

	for (Player *player : GetPlayers())
	{
		if (open)
			player->Communication()->OpenCloseNominations(true, this, this, player);
		else
			player->Communication()->OpenCloseNominations(false);
	}
}

bool Storyteller::ManageVotes(std::map<Player *, bool> votes, Player *nominee, Player *nominator)
{
	// Announce votes
	for (Player* p : GetPlayers())
		p->Communication()->AnnounceVotes(votes, nominee, nominator);

	// Count votes
	int numAlive = GetNumPlayersAlive();
	int numVotes = 0;
	int minMajority = (numAlive / 2) + (numAlive % 2);
	for (auto &[player, voted] : votes)
	{
		// Edge case if a butler chooses another butler as their master, they may get their vote counted
		// 	even if the first one was invalid. Not actually possible so not going to fix it
		if (voted)
		{
			Butler* butler = dynamic_cast<Butler*>(player->GetCharacterOrDrunkBaseCharacter());

			if (butler)
			{
				if (butler->CanButlerVote(votes, this))
				{
					++numVotes;
					player->Communication()->SendMessage("Your vote as Butler was counted");
				}
				else
				{
					player->Communication()->SendMessage("Your vote as Butler was not counted");
					votes[player] = false; // Fixes the display
				}
			}
			else
			{
				++numVotes;
			}
		}
	}

	// Test majority
	if (numVotes >= minMajority)
	{
		if (numVotes > choppingBlockVotes)
		{
			// Enough votes
			AnnounceMessage(
					nominee->PlayerName() + " is on the chopping block with " + std::to_string(numVotes) + " votes");

			choppingBlockVotes = numVotes;
			choppingBlock = nominee;
			return true;
		} else if (numVotes == choppingBlockVotes)
		{
			// Tie
			if (choppingBlock != nullptr)
			{
				AnnounceMessage(std::to_string(numVotes) + " votes for " + nominee->PlayerName() + " are tied with " +
								std::to_string(numVotes) + " votes for " + choppingBlock->PlayerName() +
								". Nobody is on the chopping block");
			} else
			{
				// Rare case: number of votes matched an existing tie
				AnnounceMessage(std::to_string(numVotes) + " votes for " + nominee->PlayerName() +
								" ties with the existing tie. Nobody is on the chopping block");
			}

			choppingBlock = nullptr;
			return false;
		} else
		{
			// Not enough votes
			AnnounceMessage(std::to_string(numVotes) + " votes for " + nominee->PlayerName() + " is less than " +
							std::to_string(choppingBlockVotes) + " votes for " + choppingBlock->PlayerName() +
							". " + choppingBlock->PlayerName() + " remains on the chopping block");
			return false;
		}
	} else
	{
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
			if (!player->IsDead() || player->HasGhostVote())
				player->Communication()->OpenCloseVoting(true, false, this, player, voteTimeSeconds);
		}
	} else
	{
		for (Player *player : GetPlayers())
		{
			if (!player->IsDead())
				player->Communication()->OpenCloseVoting(false);
		}
	}
}

void Storyteller::InformSlayerAttempt(Player *target, Player *sourcePlayer)
{
	//Note : the sourcePlayer can be dead and can not be the Slayer (bluff)
	slayActionData = {target, sourcePlayer};
	slayActionCondWaiting = false;
	slayActionCond.notify_one();
}

void Storyteller::OpenCloseDayActions(bool open)
{
	if (open)
	{
		for (Player *player : GetPlayers())
		{
			player->Communication()->OpenCloseDayActions(true, this, this, player);
		}
	}
	else
	{
		for (Player *player : GetPlayers())
		{
			player->Communication()->OpenCloseDayActions(false);
		}
	}
}

Player* Storyteller::ProcessSlayAction(Player *target, Player *sourcePlayer)
{
	Player* whoDied { nullptr };

	if (!sourcePlayer->IsDead())
		if (auto slayer = dynamic_cast<Slayer*>(sourcePlayer->GetCharacterOrDrunkBaseCharacter()))
			whoDied = slayer->AttemptSlay(target, this);

	AnnounceMessage(
			sourcePlayer->PlayerName() + " claims to slay " + target->PlayerName() +
			((whoDied != nullptr) ?
				(". " + whoDied->PlayerName() + " was killed") :
				(". Nothing happens"))
	);

	return whoDied;
}

std::tuple<bool, bool, WinType> Storyteller::CheckGameWin(bool duringDay)
{
	// Sources of game ending:
	// Only 2 players alive: evil win
	// All demons are dead: good win
	// Saint is executed: evil win
	// Only 3 players alive, no execution this day, and a mayor is alive

	int aliveDemons = 0;
	for (Player* player : GetPlayers())
	{
		if (player->GetCharacter()->GetCharacterTraits().isDemon && !player->IsDead())
			++aliveDemons;
	}

	if (aliveDemons == 0)
	{
		return std::make_tuple(true, false, WinType::DEMONS_DEAD); // Game ended, good win
	}

	if (!duringDay && HasAnyExecutionPhaseTakenPlace())
	{
		if (auto [ lastExecution, executionTime ] = GetLastExecutionDeath(); executionTime == GetCurrentTime())
		{
			if (lastExecution != nullptr)
			{
				if (lastExecution->GetCharacter()->GetCharacterType() == CharacterType::SAINT &&
					!AbilityMalfunctions(lastExecution)) // Saint doesn't lose if they are drunk/poisoned
					return std::make_tuple(true, true, WinType::SAINT_EXECUTED); // Game ended, evil win
			} else
			{
				bool functionalMayorAlive {false};
				for (Player *p : GetPlayers())
				{
					if (!p->IsDead() &&
						p->GetCharacter()->GetCharacterType() == CharacterType::MAYOR &&
						!AbilityMalfunctions(p) &&
						NumberOfPlayers() == 3)
					{
						functionalMayorAlive = true;
						break;
					}
				}

				if (functionalMayorAlive)
					return std::make_tuple(true, false, WinType::MAYOR_NO_EXECUTION); // Game ended, good win
			}
		}
	}

	if (GetNumPlayersAlive() <= 2)
	{
		return std::make_tuple(true, true, WinType::TWO_PLAYERS_LEFT_ALIVE); // Game ended, evil win
	}

	return { false, false, WinType::NONE }; // Game didn't end
}

void Storyteller::ManageWin(bool evilWin, WinType winType)
{
	assert (winType != WinType::NONE);

	//POLISH: Spectacular win messages?
	switch (winType)
	{
		case WinType::DEMONS_DEAD:
			AnnounceMessage("All demons have died. Good team wins");
			break;
		case WinType::TWO_PLAYERS_LEFT_ALIVE:
			AnnounceMessage("Two players left alive. Evil team wins");
			break;
		case WinType::SAINT_EXECUTED:
			AnnounceMessage("The Saint was executed. Evil team wins");
			break;
		case WinType::MAYOR_NO_EXECUTION:
			AnnounceMessage("No execution with Mayor. Good team wins");
			break;
		default:
			assert(false);
			AnnounceMessage(evilWin ? "Evil team wins" : "Good team wins");
			break;
	}

	//TODO: Log game history
}

}
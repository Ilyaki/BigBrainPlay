#include "DiscordPlayerCommunication.hpp"

using namespace SleepyDiscord;
using namespace TroubleBrewing;

namespace Discord
{

DiscordPlayerCommunication::DiscordPlayerCommunication(
		DiscordClient *_discordClient,
		User _user,
		SleepyDiscord::DirectMessageChannel _dmChannel) :
	discordClient{_discordClient},
	user{_user},
	dmChannel{_dmChannel},
	nominationData{std::make_tuple(false, nullptr, nullptr, nullptr)}
{
	flusherThread = std::thread([this]{this->FlusherStart();});
}

void DiscordPlayerCommunication::SendMessage(std::string msg, bool flush)
{
	const std::lock_guard<std::mutex> messageBufLock(messageBufMutex);
	messageBuf << msg << "\\n";

	if (flush)
		FlushMessage();
}

void DiscordPlayerCommunication::FlushMessage()
{
	flushCondition.notify_one();
}

TroubleBrewing::Player *
DiscordPlayerCommunication::InputPlayer(TroubleBrewing::GameState *gameState,
										TroubleBrewing::Player *sourcePlayer,
										std::string messageRequest,
										TroubleBrewing::PlayerCommunication::InputPlayerPredicate predicate)
{
	TextSeparator();

	PrintPlayerIDs(gameState);

	// e.g.: "Fortune Teller: Enter the player you want to check"
	// ** makes bold in Discord
	SendMessage("**" + messageRequest + "**", true);

	Player* foundTarget {nullptr};
	auto players = gameState->GetPlayers();

	while (foundTarget == nullptr)
	{
		bool timeout = false;
		int playerID = -1;

		while(playerID == -1)
		{
			SendMessage("Enter a player ID:");

			auto lock = std::unique_lock<std::mutex>{dmMessageConditionVarMutex};
			auto res = dmMessageConditionVar.wait_for(lock, std::chrono::seconds(60));

			if (res == std::cv_status::timeout)
			{
				// Took too long to respond
				SendMessage("Took too long to respond, selecting a random player", false);
				timeout = true;
				break;
			}

			auto message = lastMessage;

			try
			{
				playerID = std::stoi(message.content);
			}
			catch(const std::invalid_argument& e)
			{
				SendMessage("Invalid ID");
				continue;
			}
			catch(const std::out_of_range& e)
			{
				SendMessage("Invalid ID");
				continue;
			}
		}

		if (timeout)
		{
			auto pr = predicate.first;
			if (pr) // Need to match a predicate
				foundTarget = *std::ranges::find_if(players, [pr](Player* p){ return pr(p); });
			else
				foundTarget = players.front();

			SendMessage("Selected player: " + foundTarget->PlayerName());
			break;
		}

		auto index = std::find_if(players.begin(), players.end(),
								  [playerID](Player *p) { return p->MatchesPlayerID(playerID); });

		if (index == players.end()) // No player with player ID
		{
			SendMessage("No matching player found");
			continue;
		}
		else if (predicate.first && !predicate.first(*index)) // Doesn't match condition
		{
			SendMessage("Player doesn't match conditions: **" + predicate.second + "**");
			continue;
		}
		else // Found target
		{
			foundTarget = *index;
			break;
		}
	}

	TextSeparator();

	return foundTarget;
}

void DiscordPlayerCommunication::OnDirectMessage(SleepyDiscord::Message message)
{
	lastMessage = message;

	if (std::get<0>(nominationData)) // Are nominations open
		ProcessNomination(message);
	else if (std::get<0>(votingData)) // Is voting open
		ProcessVote(message);
	else if (std::get<0>(dayActionsData)) // Are day actions open
		ProcessDayAction(message);
	else
		dmMessageConditionVar.notify_one();
}

void DiscordPlayerCommunication::TextSeparator()
{
	auto cid = Snowflake<SleepyDiscord::Channel>{dmChannel.ID};
	auto last = discordClient->getMessage(cid, discordClient->getChannel(cid).cast().lastMessageID);
	if(last.error() || last.cast().author.bot)
		SendMessage(R"(```\n \n```)");
}

void DiscordPlayerCommunication::OpenCloseNominations(
		bool open, GameState *gameState, Voting *voting, Player* sourcePlayer)
{
	nominationData = std::make_tuple(open, gameState, voting, sourcePlayer);

	if (open)
	{
		PrintPlayerIDs(gameState);

		SendMessage("Nominate player X by writing `X`");
	}
}

void DiscordPlayerCommunication::ProcessNomination(SleepyDiscord::Message message)
{
	GameState* gameState = std::get<1>(nominationData);
	Voting* voting = std::get<2>(nominationData);
	Player* sourcePlayer = std::get<3>(nominationData);

	auto players = gameState->GetPlayers();

	int playerID = -1;

	try
	{
		playerID = std::stoi(message.content);
	}
	catch(const std::invalid_argument& e)
	{
		SendMessage("Invalid ID");
		return;
	}
	catch(const std::out_of_range& e)
	{
		SendMessage("Invalid ID");
		return;
	}

	if (playerID != -1)
	{
		auto index = std::find_if(players.begin(), players.end(),
								  [playerID](Player *p) { return p->MatchesPlayerID(playerID); });

		if (index == players.end()) // No player with player ID
		{
			SendMessage("No matching player found");
			return;
		} else if ((*index)->IsDead())
		{
			SendMessage("Player must be alive");
			return;
		} else // Found target
		{
			if (std::get<0>(nominationData)) // Check if nominations are still open (just in case)
				voting->InformNomination(*index, sourcePlayer);

			return;
		}
	}

}

void DiscordPlayerCommunication::PrintPlayerIDs(GameState* gameState)
{
	std::string msg = R"(```yaml\nIDs:\n)";
	for (Player* player : gameState->GetPlayers())
	{
		msg += "- " + player->PlayerName() + ": " +
				std::to_string(player->PlayerID()) +
				(player->IsDead() ? " (Dead)\\n" : "\\n");
	}
	msg += R"(```)";

	SendMessage(msg);
}

void DiscordPlayerCommunication::ProcessVote(SleepyDiscord::Message message)
{
	Voting* voting = std::get<2>(votingData);
	Player* sourcePlayer = std::get<3>(votingData);

	auto contents = message.content;
	constexpr auto npos = std::string::npos;
	if (contents.find("yes") != npos || contents.find("Yes") != npos ||
		contents.find("vote") != npos || contents.find("Vote") != npos)
	{
		std::get<0>(votingData) = false; // Close voting
		SendMessage("Vote cast");
		voting->InformVote(sourcePlayer, true);
	}
}

void DiscordPlayerCommunication::OpenCloseVoting(
		bool open, bool ghostVote,TroubleBrewing::Voting* voting, Player* sourcePlayer, int voteTimeSeconds)
{
	if (!open && std::get<0>(votingData)) // Closing voting before player casted a vote.
	{
		// Use the old votingData as the new arguments might be nullptr
		SendMessage("Took too long to respond. Assuming no vote");
		std::get<2>(votingData)->InformVote(std::get<3>(votingData), false);
	}

	votingData = std::make_tuple(open, ghostVote, voting, sourcePlayer);

	if (open)
	{
		SendMessage("Vote for " + sourcePlayer->PlayerName() + " by writing `vote` or `yes`." +
					(ghostVote ? " This will use your ghost vote." : "") +
					" You have " + std::to_string(voteTimeSeconds) + " seconds to vote");
	}
}

void DiscordPlayerCommunication::FlusherStart()
{
	while (true)
	{
		auto lock = std::unique_lock<std::mutex>{flushConditionMutex};
		flushCondition.wait(lock);

		const std::lock_guard<std::mutex> messageBufLock(messageBufMutex);

		auto c = Snowflake<SleepyDiscord::Channel>{dmChannel.ID};

		bool sent = false;

		while (!sent)
		{
			try
			{
				discordClient->sendMessage(c, messageBuf.str());
				sent = true;
			}
			catch(...)
			{
				// Discord prevents sending messages too quickly.
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}

		messageBuf.str(std::string{});
	}
}

void DiscordPlayerCommunication::OpenCloseDayActions(
		bool open, DayActions* dayActions, GameState* gameState, Player* sourcePlayer)
{
	dayActionsData = std::make_tuple(open, dayActions, gameState, sourcePlayer);

	if (open)
		SendMessage("Day actions are now open. Write `slay X` to announce slaying player X");
	else
		SendMessage("Day actions are now closed");
}

void DiscordPlayerCommunication::ProcessDayAction(SleepyDiscord::Message message)
{
	DayActions* dayActions = std::get<1>(dayActionsData);
	GameState* gameState = std::get<2>(dayActionsData);
	Player* sourcePlayer = std::get<3>(dayActionsData);
	std::string content = message.content;

std::cout << "TEST" << std::endl;
	constexpr auto npos = std::string::npos;
	if (content.find("slay ") != npos)
	{
		std::string targetString = content.substr(5); // "slay " is 5 characters

		//TODO: could move this into helper function
		auto players = gameState->GetPlayers();

		int playerID = -1;

		try
		{
			playerID = std::stoi(message.content);
		}
		catch(const std::invalid_argument& e)
		{
			SendMessage("Invalid ID");
			return;
		}
		catch(const std::out_of_range& e)
		{
			SendMessage("Invalid ID");
			return;
		}

		if (playerID != -1)
		{
			auto index = std::find_if(players.begin(), players.end(),
									  [playerID](Player *p) { return p->MatchesPlayerID(playerID); });

			if (index == players.end()) // No player with player ID
			{
				SendMessage("No matching player found");
				return;
			} else // Found target
			{
				if (std::get<0>(dayActionsData)) // Check if day actions are still open (just in case)
					dayActions->InformSlayerAttempt(*index, sourcePlayer);

				return;
			}
		}
	}
}

void DiscordPlayerCommunication::NewParagraph()
{
	SendMessage("_ _\\n\\n_ _", false);
}

void DiscordPlayerCommunication::BlankPage()
{
	SendMessage("_ _\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n"
				"\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n\\n_ _");
}

void DiscordPlayerCommunication::AnnounceVotes(
		const std::map<Player *, bool> &votes, Player *nominee, Player *nominator)
{
	std::string votesMsg = R"(```yaml\nVotes to execute )" + nominee->PlayerName() + ":\\n";
	for (auto pair : votes)
	{
		votesMsg += "- " + pair.first->PlayerName() + ": " + (pair.second ? "yes" : "no");

		if (pair.first->IsDead())
			votesMsg += ". Used ghost vote";

		votesMsg += "\\n";
	}
	votesMsg += R"(```)";

	SendMessage(votesMsg);
}

}
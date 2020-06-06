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
		int playerID = -1;

		while(playerID == -1)
		{
			SendMessage("Enter a player ID:");

			auto lock = std::unique_lock<std::mutex>{dmMessageConditionVarMutex};
			dmMessageConditionVar.wait(lock); // TODO: wait_until to prevent eternity wait

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
	// Don't use the multiline quote >>> as it may include more messages if more messages are added before a flush
	SendMessage("> Player IDs:", false);
	for (Player* player : gameState->GetPlayers())
	{
		SendMessage( "> " +	player->PlayerName() + (player->IsDead() ? " (Dead): " : ": ") + std::to_string(player->PlayerID()), false);
	}
	FlushMessage();
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

}
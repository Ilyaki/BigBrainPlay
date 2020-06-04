#include "DiscordPlayerCommunication.hpp"

using namespace SleepyDiscord;
using namespace TroubleBrewing;

namespace Discord
{

DiscordPlayerCommunication::DiscordPlayerCommunication(
		DiscordClient *_discordClient,
		User _user,
		SleepyDiscord::DirectMessageChannel _dmChannel) :
	discordClient{_discordClient}, user{_user}, dmChannel{_dmChannel}
{
}

void DiscordPlayerCommunication::SendMessage(std::string msg, bool flush)
{
	messageBuf << msg << "\\n";

	if (flush)
		FlushMessage();
}

void DiscordPlayerCommunication::FlushMessage()
{
	std::string n = dmChannel.ID;
	auto c = Snowflake<SleepyDiscord::Channel>{n};

	bool sent = false;
//TODO: replace all \n with \\n
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
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}

	messageBuf.str(std::string{});
}

TroubleBrewing::Player *
DiscordPlayerCommunication::InputPlayer(TroubleBrewing::GameState *gameState,
										TroubleBrewing::Player *sourcePlayer,
										std::string messageRequest,
										TroubleBrewing::PlayerCommunication::InputPlayerPredicate predicate)
{
	TextSeparator();

	// Print out player IDs
	SendMessage("Player IDs:", false);
	for (Player* player : gameState->GetPlayers())
	{
		SendMessage(
				player->PlayerName() +  ": " + std::to_string(player->PlayerID()), false);
	}

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
			catch(std::invalid_argument e)
			{
				SendMessage("Invalid ID");
				continue;
			}
			catch(std::out_of_range e)
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
	message.reply(discordClient, "DiscordPlayerCommunication::OnDirectMessage received");
	lastMessage = message;
	dmMessageConditionVar.notify_one();
}

void DiscordPlayerCommunication::TextSeparator()
{
	SendMessage(R"(```\n \n```)");
}

}
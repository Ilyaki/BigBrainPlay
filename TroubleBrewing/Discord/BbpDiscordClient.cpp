#include <ranges>
#include "BbpDiscordClient.hpp"
#include "../Storyteller.hpp"

using namespace TroubleBrewing;

namespace Discord
{

void BbpDiscordClient::onMessage(SleepyDiscord::Message message)
{
	auto channelIdString = message.channelID.string();
	if (!message.author.bot && communicationMap.contains(channelIdString))
	{
		communicationMap.at(channelIdString)->OnDirectMessage(message);
	}

	if (!message.author.bot && message.startsWith("BBP start"))
	{
		auto c = message.channelID;
		auto mentions = message.mentions | std::views::filter([](const SleepyDiscord::User& u){return !u.bot; });

		if (std::ranges::distance(mentions) != 0)
		{
			std::string startMsg = "Starting with: ";

			std::vector<std::pair<PlayerData,
					std::shared_ptr<PlayerCommunication>>> playerDatas;

			int playerID = 1;
			for (SleepyDiscord::User user : mentions)
			{
				startMsg += user.username + " ";

				auto dm = createDirectMessageChannel(user.ID);
				assert(!dm.error());//TODO: make dm.error() prettier
				SleepyDiscord::DMChannel dmChannel = dm.cast();

				PlayerData playerData{user.username, playerID};
				auto comm = std::make_shared<DiscordPlayerCommunication>(this, user, dmChannel);

				playerDatas.push_back({playerData, comm});

				communicationMap.insert({dmChannel.ID.string(), comm});

				++playerID;
			}

			sendMessage(c, startMsg);

			storytellerThread = std::thread(StartStorytellerThread, playerDatas);
		}
	}
}

void BbpDiscordClient::StartStorytellerThread(std::vector<std::pair<PlayerData,
		std::shared_ptr<PlayerCommunication>>> playerDatas)
{
	Storyteller storyteller{playerDatas};
	storyteller.StartGame();
}

}
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
		communicationMap.at(channelIdString).first->OnDirectMessage(message);
	}

	if (!message.author.bot && message.startsWith("BBP start"))
	{
		auto c = message.channelID;
		auto mentions = message.mentions | std::views::filter([](const SleepyDiscord::User& u){return !u.bot; });

		if (std::ranges::distance(mentions) != 0)
		{
			//TODO: need to have multiple games running?
			if (storytellerThread.joinable())
				storytellerThread.join();

			std::string startMsg = "Starting with: ";

			++currentStorytellerID;

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

				communicationMap.insert({ dmChannel.ID.string(), {comm, currentStorytellerID }});

				++playerID;
			}

			sendMessage(c, startMsg);

			storytellerThread = std::thread(StartStorytellerThread, this, playerDatas, currentStorytellerID);
		}
	}
}

void BbpDiscordClient::StartStorytellerThread(BbpDiscordClient* ptr, std::vector<std::pair<PlayerData,
		std::shared_ptr<PlayerCommunication>>> playerDatas, int storytellerID)
{
	Storyteller storyteller{playerDatas};
	storyteller.StartGame();

	// Game finished. Remove communication maps with our storyteller
	std::vector<CommunicationMapType::const_iterator> iterators{};
	for (auto iter = ptr->communicationMap.cbegin(); iter != ptr->communicationMap.cend(); ++iter)
	{
		if ((*iter).second.second == storytellerID)
			iterators.push_back(iter);
	}

	for (const auto& iter : iterators)
	{
		ptr->communicationMap.erase(iter);
	}
}

}
#ifndef BBP_CONSOLE_BBPDISCORDCLIENT_HPP
#define BBP_CONSOLE_BBPDISCORDCLIENT_HPP

#include <iostream>
#include "sleepy_discord/websocketpp_websocket.h"
#include "DiscordPlayerCommunication.hpp"
#include "../Storyteller.hpp"

namespace Discord
{

class BbpDiscordClient: public SleepyDiscord::DiscordClient
{
	int currentStorytellerID {0 };

	using CommunicationMapType = std::map<std::string, std::pair<std::shared_ptr<DiscordPlayerCommunication>, int>>;

	std::thread storytellerThread{};

	static void StartStorytellerThread(BbpDiscordClient* ptr, std::vector<std::pair<TroubleBrewing::PlayerData,
			std::shared_ptr<TroubleBrewing::PlayerCommunication>>> playerDatas, int storytellerID);

public:
	using SleepyDiscord::DiscordClient::WebsocketppDiscordClient;

	CommunicationMapType communicationMap{};

	void onMessage(SleepyDiscord::Message message) override;
};

}


#endif //BBP_CONSOLE_BBPDISCORDCLIENT_HPP

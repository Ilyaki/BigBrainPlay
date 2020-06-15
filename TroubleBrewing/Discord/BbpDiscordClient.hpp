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
	std::map<std::string, std::shared_ptr<DiscordPlayerCommunication>> communicationMap{};

	std::thread storytellerThread{};

	static void StartStorytellerThread(std::vector<std::pair<TroubleBrewing::PlayerData,
			std::shared_ptr<TroubleBrewing::PlayerCommunication>>> playerDatas);

public:
	using SleepyDiscord::DiscordClient::WebsocketppDiscordClient;

	void onMessage(SleepyDiscord::Message message) override;
};

}


#endif //BBP_CONSOLE_BBPDISCORDCLIENT_HPP

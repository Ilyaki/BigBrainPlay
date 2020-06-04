#ifndef BBP_CONSOLE_DISCORDPLAYERCOMMUNICATION_HPP
#define BBP_CONSOLE_DISCORDPLAYERCOMMUNICATION_HPP

#include <condition_variable>
#include <sleepy_discord/websocketpp_websocket.h>
#include "../PlayerCommunication.hpp"

namespace Discord
{

class DiscordPlayerCommunication : public TroubleBrewing::PlayerCommunication
{
	SleepyDiscord::DiscordClient* discordClient;
	const SleepyDiscord::User user;
	SleepyDiscord::DMChannel dmChannel;

	std::condition_variable dmMessageConditionVar;
	std::mutex dmMessageConditionVarMutex;
	SleepyDiscord::Message lastMessage;

	std::stringstream messageBuf;

	void TextSeparator();

	void FlushMessage();

public:
	DiscordPlayerCommunication(
			SleepyDiscord::DiscordClient* _discordClient,
			SleepyDiscord::User _user,
			SleepyDiscord::DirectMessageChannel _dmChannel);

	void OnDirectMessage(SleepyDiscord::Message message);

	TroubleBrewing::Player* InputPlayer(
			TroubleBrewing::GameState* gameState,
			TroubleBrewing::Player* sourcePlayer,
			std::string messageRequest,
			InputPlayerPredicate predicate
	) override;

	void SendMessage(std::string msg, bool flush = true) override;
};

}

#endif //BBP_CONSOLE_DISCORDPLAYERCOMMUNICATION_HPP

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

	// is nominations open, game state, voting interface, source player
	std::tuple<bool, TroubleBrewing::GameState*, TroubleBrewing::Voting*, TroubleBrewing::Player*> nominationData;

	// is voting open, is ghost vote, voting interface
	std::tuple<bool, bool, TroubleBrewing::Voting*, TroubleBrewing::Player*> votingData;

	void TextSeparator();
	void FlushMessage();

	void PrintPlayerIDs(TroubleBrewing::GameState* gameState);

	void ProcessNomination(SleepyDiscord::Message message);
	void ProcessVote(SleepyDiscord::Message message);

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

	void OpenCloseNominations(bool open,
			TroubleBrewing::GameState* gameState,
			TroubleBrewing::Voting* voting,
			TroubleBrewing::Player* sourcePlayer) override;

	virtual void OpenCloseVoting(bool open, bool ghostVote,TroubleBrewing::Voting* voting,
			Player* sourcePlayer, int voteTimeSeconds) override;
};

}

#endif //BBP_CONSOLE_DISCORDPLAYERCOMMUNICATION_HPP

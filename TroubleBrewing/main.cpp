#include <iostream>
#include "sleepy_discord/websocketpp_websocket.h"
#include "Discord/DiscordPlayerCommunication.hpp"
#include "Discord/BbpDiscordClient.hpp"

using namespace TroubleBrewing;

int main()
{
	//TODO: needs to be input from file
	Discord::BbpDiscordClient client("NzE3NDI4MTQyODg0NzgyMTcx.XtnPuQ.nRdj9zMkACdtAEjUf6Ge-atCTxs", 2);
	client.run();

	return 0;
}

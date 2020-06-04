#include <iostream>
#include "sleepy_discord/websocketpp_websocket.h"
#include "Discord/DiscordPlayerCommunication.hpp"
#include "Discord/BbpDiscordClient.hpp"

using namespace TroubleBrewing;


int main()
{
	Discord::BbpDiscordClient client("NzE3NDI4MTQyODg0NzgyMTcx.XtaLSA.qrQhKNXCFvBbQ78nAB6CwG5pcIQ", 2);
	client.run();

	return 0;
}

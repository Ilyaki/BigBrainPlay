#include "Discord/BbpDiscordClient.hpp"

//FIXME: in cpr/cpr/session.cpp, comment
//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
// as it is a security risk

int main()
{
	if (std::ifstream tokenFile { "token.txt" }; tokenFile.is_open())
	{
		std::string token;
		std::getline(tokenFile, token);
		Discord::BbpDiscordClient client(token, 2);
		client.run();
	}
	else
	{
		std::cerr << "Couldn't open token.txt" << std::endl;
	}

	return 0;
}
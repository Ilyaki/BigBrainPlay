#ifndef BBP_CONSOLE_STORYTELLER_HPP
#define BBP_CONSOLE_STORYTELLER_HPP

#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>
#include "Characters/CharacterType.hpp"
#include "GameState.hpp"

namespace TroubleBrewing
{

class Storyteller : public GameState, Voting
{
	// Orders
	const std::vector<CharacterType> zerothNightOrder
	{
		CharacterType::TEST_CHARACTER,
		CharacterType::WASHERWOMAN,
		CharacterType::LIBRARIAN,
		CharacterType::INVESTIGATOR,
		CharacterType::CHEF,
		CharacterType::EMPATH,
		CharacterType::FORTUNETELLER
	};

	const std::vector<CharacterType> nightOrder
	{
			CharacterType::TEST_CHARACTER,
			CharacterType::MONK,
			CharacterType::EMPATH,
			CharacterType::FORTUNETELLER,
			CharacterType::UNDERTAKER
	};

	const std::vector<CharacterType> charactersInPlay
	{
			CharacterType::RAVENKEEPER,
		//	CharacterType::CHEF
			/*CharacterType::TEST_CHARACTER,
			CharacterType::WASHERWOMAN,
			CharacterType::CHEF,
			CharacterType::LIBRARIAN,
			CharacterType::EMPATH,
			CharacterType::INVESTIGATOR,
			CharacterType::FORTUNETELLER,*/
	};

	// Voting system
	std::map<Player*, bool> votes;
	Player* choppingBlock;
	int choppingBlockVotes;
	bool nominationsOpen;

	std::condition_variable informNominationCond;
	std::mutex informNominationCondMutex;
	std::tuple<Player*, Player*> informNominationData; // nominee, nominator

	// Implementation functions
	void NightPhase(const std::vector<CharacterType> order, int night);
	void DayPhase(int day);

	void AnnounceMessage(std::string message, bool flush = true);

	void OpenCloseNominations(bool open);
	void OpenCloseVoting(bool open, int voteTimeSeconds = 0);

	void ProcessNomination(Player* nominee, Player* nominator);

	void ExecuteChoppingBlock();
	bool ManageVotes(std::map<Player*, bool> votes, Player* nominee, Player* nominator);


public:
	Storyteller(std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>> playerDatas);

	void StartGame();

	bool GameFinished() const;

	void InformNomination(Player* nominee, Player* nominator) override;
	void InformVote(Player* sourcePlayer, bool vote) override;
};

}


#endif //BBP_CONSOLE_STORYTELLER_HPP

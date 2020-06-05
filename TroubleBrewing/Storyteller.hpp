#ifndef BBP_CONSOLE_STORYTELLER_HPP
#define BBP_CONSOLE_STORYTELLER_HPP

#include <vector>
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
			CharacterType::EMPATH,
			CharacterType::FORTUNETELLER
	};

	const std::vector<CharacterType> charactersInPlay
	{
			CharacterType::TEST_CHARACTER,
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

	// Implementation functions
	void NightPhase(const std::vector<CharacterType> order, int night);
	void DayPhase(int day);

	void AnnounceMessage(std::string message, bool flush = true);

	void OpenCloseNominations(bool open);
	void OpenCloseVoting(bool open, int voteTimeSeconds = 0);

	void ExecuteChoppingBlock();
	bool ManageVotes(std::map<Player*, bool> votes, Player* nominee, Player* nominator);


public:
	Storyteller(std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>> playerDatas);

	void StartGame();

	bool GameFinished();

	void InformNomination(Player* nominee, Player* nominator) override;
	void InformVote(Player* sourcePlayer, bool vote) override;
};

}


#endif //BBP_CONSOLE_STORYTELLER_HPP

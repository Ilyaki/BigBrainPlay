#ifndef BBP_CONSOLE_STORYTELLER_HPP
#define BBP_CONSOLE_STORYTELLER_HPP

#include <vector>
#include <map>
#include <mutex>
#include <condition_variable>
#include "Characters/CharacterType.hpp"
#include "GameState.hpp"
#include "Characters/Washerwoman.hpp"
#include "Characters/Librarian.hpp"
#include "Characters/Investigator.hpp"
#include "Characters/Chef.hpp"
#include "Characters/Empath.hpp"
#include "Characters/FortuneTeller.hpp"
#include "Characters/Undertaker.hpp"
#include "Characters/Monk.hpp"
#include "Characters/Ravenkeeper.hpp"
#include "Characters/Virgin.hpp"
#include "Characters/Slayer.hpp"
#include "Characters/Soldier.hpp"
#include "Characters/TestCharacter.hpp"
#include "Characters/Poisoner.hpp"
#include "Characters/Drunk.hpp"

namespace TroubleBrewing
{

class Storyteller : public GameState, Voting, DayActions
{
	// Orders
	const std::vector<CharacterType> zerothNightOrder
	{
		CharacterType::TEST_CHARACTER,
		CharacterType::POISONER,
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
			CharacterType::POISONER,
			CharacterType::MONK,
			CharacterType::EMPATH,
			CharacterType::FORTUNETELLER,
			CharacterType::UNDERTAKER
	};

	const std::vector<CharacterType> charactersInPlay
	{
			CharacterType::POISONER,
			CharacterType::RAVENKEEPER,
			CharacterType::TEST_CHARACTER
		//	CharacterType::CHEF
			/*CharacterType::TEST_CHARACTER,
			CharacterType::WASHERWOMAN,
			CharacterType::CHEF,
			CharacterType::LIBRARIAN,
			CharacterType::EMPATH,
			CharacterType::INVESTIGATOR,
			CharacterType::FORTUNETELLER,*/
	};

	CharacterTypeTraitsMap characterTypeTraitsMap {GetCharacterTypeTraitMapTemplate<
			TestCharacter,
			Washerwoman,
			Librarian,
			Investigator,
			Chef,
			Empath,
			FortuneTeller,
			Undertaker,
			Monk,
			Ravenkeeper,
			Virgin,
			Slayer,
			Soldier,
			Poisoner,
			Drunk
	>()};


	// Voting system
	std::map<Player*, bool> votes;
	Player* choppingBlock;
	int choppingBlockVotes;
	bool nominationsOpen{false};

	// Inform nomination
	std::condition_variable informNominationCond;
	std::mutex informNominationCondMutex;
	std::tuple<Player*, Player*> informNominationData; // nominee, nominator

	// Inform slay
	// If we need to add more day actions, should change this to dayActionData, etc
	std::condition_variable slayActionCond{};
	std::mutex slayActionCondMutex{};
	std::tuple<Player*, Player*> slayActionData; // target, slayer

	void NightPhase(const std::vector<CharacterType> order, int night);
	void DayPhase(int day);

	void AnnounceMessage(std::string message, bool flush = true);

	void OpenCloseNominations(bool open);
	void OpenCloseVoting(bool open, int voteTimeSeconds = 0);
	void OpenCloseDayActions(bool open);

	/// \return true if the nomination phase should end abruptly (e.g. because Virgin was nominated)
	bool ProcessNomination(Player* nominee, Player* nominator);

	/// \return true if someone was killed
	bool ProcessSlayAction(Player* target, Player* sourcePlayer);

	void ExecuteChoppingBlock();
	bool ManageVotes(std::map<Player*, bool> votes, Player* nominee, Player* nominator);

public:
	Storyteller(std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>> playerDatas);

	void StartGame();

	void InformNomination(Player* nominee, Player* nominator) override;
	void InformVote(Player* sourcePlayer, bool vote) override;

	void InformSlayerAttempt(Player* target, Player* sourcePlayer);

	const CharacterTypeTraitsMap* GetCharacterTypeTraitsMap() override;
};

}


#endif //BBP_CONSOLE_STORYTELLER_HPP

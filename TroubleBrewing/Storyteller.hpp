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
#include "Characters/Poisoner.hpp"
#include "Characters/Butler.hpp"
#include "Characters/Drunk.hpp"
#include "Characters/Recluse.hpp"
#include "Characters/Imp.hpp"
#include "Characters/Saint.hpp"
#include "Characters/Mayor.hpp"
#include "Characters/Spy.hpp"
#include "Characters/Baron.hpp"

namespace TroubleBrewing
{

enum class WinType { NONE, TWO_PLAYERS_LEFT_ALIVE, DEMONS_DEAD, SAINT_EXECUTED, MAYOR_NO_EXECUTION };

class Storyteller : public GameState, Voting, DayActions
{
	// Orders
	const std::vector<CharacterType> zerothNightOrder
	{
		CharacterType::POISONER,
		CharacterType::WASHERWOMAN,
		CharacterType::LIBRARIAN,
		CharacterType::INVESTIGATOR,
		CharacterType::CHEF,
		CharacterType::EMPATH,
		CharacterType::FORTUNETELLER,
		CharacterType::SPY,
		CharacterType::BUTLER
	};

	const std::vector<CharacterType> nightOrder
	{
			CharacterType::POISONER,
			CharacterType::MONK,
			CharacterType::SPY,
			CharacterType::IMP,
			CharacterType::EMPATH,
			CharacterType::FORTUNETELLER,
			CharacterType::UNDERTAKER,
			CharacterType::BUTLER
	};

	const std::vector<CharacterType> charactersInPlay
	{
			CharacterType::IMP,
			CharacterType::POISONER,
			CharacterType::SPY
	};

	CharacterTypeTraitsMap characterTypeTraitsMap {GetCharacterTypeTraitMapTemplate<
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
			Mayor,
			Butler,
			Drunk,
			Recluse,
			Saint,
			Poisoner,
			Spy,
			Baron,
			Imp
	>()};


	// Voting system
	std::map<Player*, bool> votes{};
	Player* choppingBlock{};
	int choppingBlockVotes{};
	bool nominationsOpen{false};

	// Inform nomination
	std::condition_variable informNominationCond{};
	bool informNominationCondWaiting { false };
	std::mutex informNominationCondMutex{};
	std::tuple<Player*, Player*> informNominationData; // nominee, nominator

	// Inform slay
	// If we need to add more day actions, should change this to dayActionData, etc
	std::condition_variable slayActionCond{};
	bool slayActionCondWaiting { false };
	std::mutex slayActionCondMutex{};
	std::tuple<Player*, Player*> slayActionData; // target, slayer

	void NightPhase(const std::vector<CharacterType> order, int night);
	void DayPhase(int day);

	void AnnounceMessage(const std::string& message, bool flush = true);

	void AnnounceNightDeaths();

	/// \return { game ended, evil win, win type }
	std::tuple<bool, bool, WinType> CheckGameWin(bool duringDay);
	void ManageWin(bool evilWin, WinType winType);

	void OpenCloseNominations(bool open);
	void OpenCloseVoting(bool open, int voteTimeSeconds = 0);
	void OpenCloseDayActions(bool open);

	/// \return true if the nomination phase should end abruptly (e.g. because Virgin was nominated)
	bool ProcessNomination(Player* nominee, Player* nominator);

	/// \return player who was killed. nullptr if no one died
	Player* ProcessSlayAction(Player* target, Player* sourcePlayer);

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

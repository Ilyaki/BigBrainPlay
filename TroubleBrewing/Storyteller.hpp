#ifndef BBP_CONSOLE_STORYTELLER_HPP
#define BBP_CONSOLE_STORYTELLER_HPP

#include <vector>
#include "Characters/CharacterType.hpp"
#include "GameState.hpp"

namespace TroubleBrewing
{

class Storyteller : public GameState
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
			CharacterType::FORTUNETELLER,
		//	CharacterType::CHEF
			/*CharacterType::TEST_CHARACTER,
			CharacterType::WASHERWOMAN,
			CharacterType::CHEF,
			CharacterType::LIBRARIAN,
			CharacterType::EMPATH,
			CharacterType::INVESTIGATOR,
			CharacterType::FORTUNETELLER,*/
	};

	// Implementation functions
	void NightPhase(const std::vector<CharacterType> order, int night);

public:
	Storyteller(std::vector<std::pair<PlayerData, std::shared_ptr<PlayerCommunication>>> playerDatas);

	void StartGame();

	bool GameFinished();
};

}


#endif //BBP_CONSOLE_STORYTELLER_HPP

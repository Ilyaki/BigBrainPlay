#include <cassert>
#include "CharacterHelper.hpp"
#include "Washerwoman.hpp"
#include "Librarian.hpp"
#include "Investigator.hpp"
#include "Chef.hpp"
#include "Empath.hpp"
#include "FortuneTeller.hpp"
#include "Undertaker.hpp"
#include "Monk.hpp"
#include "Ravenkeeper.hpp"
#include "Virgin.hpp"
#include "Soldier.hpp"
#include "Slayer.hpp"
#include "Poisoner.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "Recluse.hpp"
#include "Imp.hpp"
#include "Butler.hpp"
#include "Drunk.hpp"
#include "Saint.hpp"
#include "Mayor.hpp"

namespace TroubleBrewing
{
std::shared_ptr<Character> CharacterHelper::CreateCharacter(CharacterType characterType, GameState* gameState,
		Player* player, bool isDrunk)
{
	std::shared_ptr<Character> character;
	Time creationTime = gameState->GetCurrentTime();

	switch (characterType)
	{
		assert(characterType != CharacterType::NO_CHARACTER);

		case CharacterType::WASHERWOMAN:
			character = std::make_shared<Washerwoman>(player, isDrunk, creationTime);
			break;

		case CharacterType::LIBRARIAN:
			character = std::make_shared<Librarian>(player, isDrunk, creationTime);
			break;

		case CharacterType::INVESTIGATOR:
			character = std::make_shared<Investigator>(player, isDrunk, creationTime);
			break;

		case CharacterType::CHEF:
			character = std::make_shared<Chef>(player, isDrunk, creationTime);
			break;

		case CharacterType::EMPATH:
			character = std::make_shared<Empath>(player, isDrunk, creationTime);
			break;

		case CharacterType::FORTUNETELLER:
			character = std::make_shared<FortuneTeller>(player, isDrunk, creationTime);
			break;

		case CharacterType::UNDERTAKER:
			character = std::make_shared<Undertaker>(player, isDrunk, creationTime);
			break;

		case CharacterType::MONK:
			character = std::make_shared<Monk>(player, isDrunk, creationTime);
			break;

		case CharacterType::RAVENKEEPER:
			character = std::make_shared<Ravenkeeper>(player, isDrunk, creationTime);
			break;

		case CharacterType::VIRGIN:
			character = std::make_shared<Virgin>(player, isDrunk, creationTime);
			break;

		case CharacterType::SOLDIER:
			character = std::make_shared<Soldier>(player, isDrunk, creationTime);
			break;

		case CharacterType::SLAYER:
			character = std::make_shared<Slayer>(player, isDrunk, creationTime);
			break;

		case CharacterType::MAYOR:
			character = std::make_shared<Mayor>(player, isDrunk, creationTime);
			break;

		case CharacterType::BUTLER:
			character = std::make_shared<Butler>(player, isDrunk, creationTime);
			break;

		case CharacterType::DRUNK:
			character = std::make_shared<Drunk>(player, gameState, creationTime);
			break;

		case CharacterType::RECLUSE:
			character = std::make_shared<Recluse>(player, isDrunk, creationTime);
			break;

		case CharacterType::SAINT:
			character = std::make_shared<Saint>(player, isDrunk, creationTime);
			break;

		case CharacterType::POISONER:
			character = std::make_shared<Poisoner>(player, isDrunk, creationTime);
			break;

		case CharacterType::SPY:
			character = std::make_shared<Spy>(player, isDrunk, creationTime);
			break;

		case CharacterType::BARON:
			character = std::make_shared<Baron>(player, isDrunk, creationTime);
			break;

		case CharacterType::IMP:
			character = std::make_shared<Imp>(player, isDrunk, creationTime);
			break;

		default:
			break;
	}

	assert(character != nullptr);

	return character;
}

}
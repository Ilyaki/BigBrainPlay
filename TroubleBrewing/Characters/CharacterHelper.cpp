#include <cassert>
#include "CharacterHelper.hpp"
#include "TestCharacter.hpp"
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
#include "Drunk.hpp"

namespace TroubleBrewing
{
std::shared_ptr<Character> CharacterHelper::CreateCharacter(CharacterType characterType, GameState* gameState,
		Player* player, bool isDrunk)
{
	std::shared_ptr<Character> character;

	switch (characterType)
	{
		//TODO: cleanup somehow since all the constructors are similar?
		assert(characterType != CharacterType::NO_CHARACTER);

		case CharacterType::TEST_CHARACTER:
			character = std::make_shared<TestCharacter>(player, isDrunk);
			break;

		case CharacterType::WASHERWOMAN:
			character = std::make_shared<Washerwoman>(player, isDrunk);
			break;

		case CharacterType::LIBRARIAN:
			character = std::make_shared<Librarian>(player, isDrunk);
			break;

		case CharacterType::INVESTIGATOR:
			character = std::make_shared<Investigator>(player, isDrunk);
			break;

		case CharacterType::CHEF:
			character = std::make_shared<Chef>(player, isDrunk);
			break;

		case CharacterType::EMPATH:
			character = std::make_shared<Empath>(player, isDrunk);
			break;

		case CharacterType::FORTUNETELLER:
			character = std::make_shared<FortuneTeller>(player, isDrunk);
			break;

		case CharacterType::UNDERTAKER:
			character = std::make_shared<Undertaker>(player, isDrunk);
			break;

		case CharacterType::MONK:
			character = std::make_shared<Monk>(player, isDrunk);
			break;

		case CharacterType::RAVENKEEPER:
			character = std::make_shared<Ravenkeeper>(player, isDrunk);
			break;

		case CharacterType::VIRGIN:
			character = std::make_shared<Virgin>(player, isDrunk);
			break;

		case CharacterType::SOLDIER:
			character = std::make_shared<Soldier>(player, isDrunk);
			break;

		case CharacterType::SLAYER:
			character = std::make_shared<Slayer>(player, isDrunk);
			break;

		case CharacterType::POISONER:
			character = std::make_shared<Poisoner>(player, isDrunk);
			break;

		case CharacterType::DRUNK:
			character = std::make_shared<Drunk>(player, gameState);
			break;

		default:
			break;
	}

	assert(character != nullptr);

	return character;
}

}
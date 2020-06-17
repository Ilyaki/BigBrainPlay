#include "CharacterMap.hpp"
#include <mutex>
#include <condition_variable>
#include "../GameState.hpp"
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
#include "Slayer.hpp"
#include "Soldier.hpp"
#include "Poisoner.hpp"
#include "Butler.hpp"
#include "Drunk.hpp"
#include "Recluse.hpp"
#include "Imp.hpp"
#include "Saint.hpp"
#include "Mayor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "CharacterTraits.hpp"

const std::map<CharacterType, std::pair<TroubleBrewing::CharacterTraits, std::string_view>>*
TroubleBrewing::CharacterMap::GetCharacterTypeTraitsMap()
{
	static std::map<CharacterType, std::pair<CharacterTraits, std::string_view>>
	characterTypeTraitsMap {GetCharacterTypeTraitMapTemplate<
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

	return &characterTypeTraitsMap;
}

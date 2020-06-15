#ifndef BBP_CHARACTERTYPEGETTER_HPP
#define BBP_CHARACTERTYPEGETTER_HPP

#include "CharacterType.hpp"

namespace TroubleBrewing
{

template<CharacterType C>
struct GetCharacterTypeT{};

#define TROUBLEBREWING_GET_CHARACTER_MAP(ENUM, TYPE) class TYPE; \
template<> \
struct GetCharacterTypeT<CharacterType::ENUM> \
{ using type = TYPE; };

TROUBLEBREWING_GET_CHARACTER_MAP(WASHERWOMAN, Washerwoman)
TROUBLEBREWING_GET_CHARACTER_MAP(LIBRARIAN, Librarian)
TROUBLEBREWING_GET_CHARACTER_MAP(INVESTIGATOR, Investigator)
TROUBLEBREWING_GET_CHARACTER_MAP(CHEF, Chef)
TROUBLEBREWING_GET_CHARACTER_MAP(EMPATH, Empath)
TROUBLEBREWING_GET_CHARACTER_MAP(FORTUNETELLER, FortuneTeller)
TROUBLEBREWING_GET_CHARACTER_MAP(UNDERTAKER, Undertaker)
TROUBLEBREWING_GET_CHARACTER_MAP(MONK, Monk)
TROUBLEBREWING_GET_CHARACTER_MAP(RAVENKEEPER, Ravenkeeper)
TROUBLEBREWING_GET_CHARACTER_MAP(VIRGIN, Virgin)
TROUBLEBREWING_GET_CHARACTER_MAP(SLAYER, Slayer)
TROUBLEBREWING_GET_CHARACTER_MAP(SOLDIER, Soldier)
TROUBLEBREWING_GET_CHARACTER_MAP(MAYOR, Mayor)

TROUBLEBREWING_GET_CHARACTER_MAP(BUTLER, Butler)
TROUBLEBREWING_GET_CHARACTER_MAP(DRUNK, Drunk)
TROUBLEBREWING_GET_CHARACTER_MAP(RECLUSE, Recluse)
TROUBLEBREWING_GET_CHARACTER_MAP(SAINT, Saint)

TROUBLEBREWING_GET_CHARACTER_MAP(POISONER, Poisoner)
TROUBLEBREWING_GET_CHARACTER_MAP(SPY, Spy)

TROUBLEBREWING_GET_CHARACTER_MAP(IMP, Imp)

#undef TROUBLEBREWING_GET_CHARACTER_MAP

template<CharacterType C>
using GetCharacterType = typename GetCharacterTypeT<C>::type;

}

#endif //BBP_CHARACTERTYPEGETTER_HPP

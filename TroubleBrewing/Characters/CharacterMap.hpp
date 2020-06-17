#ifndef BBP_CHARACTERMAP_HPP
#define BBP_CHARACTERMAP_HPP

#include <string_view>
#include "CharacterTraits.hpp"

namespace TroubleBrewing::CharacterMap
{

const std::map<CharacterType, std::pair<CharacterTraits, std::string_view>>* GetCharacterTypeTraitsMap();

}

#endif //BBP_CHARACTERMAP_HPP

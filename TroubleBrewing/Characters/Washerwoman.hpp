#ifndef BBP_CONSOLE_WASHERWOMAN_HPP
#define BBP_CONSOLE_WASHERWOMAN_HPP

#include "CharacterKnowerBase.hpp"

namespace TroubleBrewing
{

class Washerwoman : public CharacterKnowerBase
{
public:
	Washerwoman(Player* player) : CharacterKnowerBase(player){}

	static constexpr CharacterType CharType = CharacterType::WASHERWOMAN;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Washerwoman";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	constexpr CharacterTraits TargetTraits() override
	{
		return CharacterTraits::Townsfolk();
	}
};

}

#endif //BBP_CONSOLE_WASHERWOMAN_HPP

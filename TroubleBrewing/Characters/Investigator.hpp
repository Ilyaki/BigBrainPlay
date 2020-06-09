#ifndef BBP_CONSOLE_INVESTIGATOR_HPP
#define BBP_CONSOLE_INVESTIGATOR_HPP

#include "CharacterKnowerBase.hpp"

namespace TroubleBrewing
{

class Investigator : public CharacterKnowerBase
{
public:
	Investigator(Player* player) : CharacterKnowerBase(player){}

	static constexpr CharacterType CharType = CharacterType::INVESTIGATOR;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Investigator";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	constexpr CharacterTraits TargetTraits() override
	{
		return CharacterTraits::Minion();
	}
};

}


#endif //BBP_CONSOLE_INVESTIGATOR_HPP

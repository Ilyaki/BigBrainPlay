#ifndef BBP_CONSOLE_LIBRARIAN_HPP
#define BBP_CONSOLE_LIBRARIAN_HPP


#include "CharacterKnowerBase.hpp"

namespace TroubleBrewing
{

class Librarian : public CharacterKnowerBase
{
public:
	Librarian(Player* player) : CharacterKnowerBase(player){}

	static constexpr CharacterType CharType = CharacterType::LIBRARIAN;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Townsfolk();
	static constexpr std::string_view CharName  = "Librarian";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	constexpr CharacterTraits TargetTraits() override
	{
		return CharacterTraits::Outsider();
	}
};

}

#endif //BBP_CONSOLE_LIBRARIAN_HPP

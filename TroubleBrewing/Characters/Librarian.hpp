#ifndef BBP_CONSOLE_LIBRARIAN_HPP
#define BBP_CONSOLE_LIBRARIAN_HPP


#include "CharacterKnowerBase.hpp"

namespace TroubleBrewing
{

class Librarian : public CharacterKnowerBase
{
public:
	explicit Librarian(Player* _player);

	constexpr CharacterTraits TargetTraits() override
	{
		return CharacterTraits::Outsider();
	}

	std::string GetCharacterName() override;
	CharacterType GetCharacterType() override;
};

}

#endif //BBP_CONSOLE_LIBRARIAN_HPP

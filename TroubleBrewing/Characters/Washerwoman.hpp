#ifndef BBP_CONSOLE_WASHERWOMAN_HPP
#define BBP_CONSOLE_WASHERWOMAN_HPP

#include "CharacterKnowerBase.hpp"

namespace TroubleBrewing
{

class Washerwoman : public CharacterKnowerBase
{
public:
	explicit Washerwoman(Player* _player);

	constexpr CharacterTraits TargetTraits() override
	{
		return CharacterTraits::Townsfolk();
	}

	std::string GetCharacterName() override;
	CharacterType GetCharacterType() override;
};

}

#endif //BBP_CONSOLE_WASHERWOMAN_HPP

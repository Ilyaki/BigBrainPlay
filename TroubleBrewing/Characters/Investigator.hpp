#ifndef BBP_CONSOLE_INVESTIGATOR_HPP
#define BBP_CONSOLE_INVESTIGATOR_HPP

#include "CharacterKnowerBase.hpp"

namespace TroubleBrewing
{

class Investigator : public CharacterKnowerBase
{
public:
	explicit Investigator(Player* _player);

	constexpr CharacterTraits TargetTraits() override
	{
		return CharacterTraits::Minion();
	}
};

}


#endif //BBP_CONSOLE_INVESTIGATOR_HPP

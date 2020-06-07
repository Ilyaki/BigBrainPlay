#ifndef BBP_VIRGIN_HPP
#define BBP_VIRGIN_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Virgin : public Character
{
	bool usedVirginAbility;

public:
	explicit Virgin(Player* _player);

	/// VirginAbility: Should be called whenever the Virgin is nominated.
	/// \returns True if the nominator should be executed
	bool VirginAbility(Player* nominator);
};

}

#endif //BBP_VIRGIN_HPP

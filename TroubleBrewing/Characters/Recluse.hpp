#ifndef BBP_RECLUSE_HPP
#define BBP_RECLUSE_HPP

#include "Character.hpp"

namespace TroubleBrewing
{

class Recluse : public Character
{
public:
	using Character::Character;

	static constexpr CharacterType CharType = CharacterType::RECLUSE;
	static constexpr CharacterTraits CharTraits = CharacterTraits::Outsider();
	static constexpr std::string_view CharName  = "Recluse";
	TROUBLEBREWING_CHARACTER_CONSTEXPR_GETTERS

	PerceivedCharacterData GeneratePerceivedCharacterData(GameState* gameState) override;
};

}

#endif //BBP_RECLUSE_HPP

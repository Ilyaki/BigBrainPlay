#ifndef BBP_CHARACTERCHOOSER_HPP
#define BBP_CHARACTERCHOOSER_HPP

#include <vector>
#include <memory>
#include "CharacterType.hpp"
#include "../PlayerData.hpp"

namespace TroubleBrewing
{
class PlayerCommunication;
}

namespace TroubleBrewing::CharacterChooser
{

struct CharacterCount
{
	int townsfolk;
	int outsiders;
	int minions;
	int demons;

	constexpr CharacterCount(int _townsfolk, int _outsiders, int _minions, int _demons) :
		townsfolk{_townsfolk}, outsiders{_outsiders}, minions{_minions}, demons{_demons}{}
};

/// \return { success, chosen characters, expected character count }
std::tuple<bool, std::vector<CharacterType>, CharacterCount> GenerateCharacterTypes(
		const std::vector<std::pair<PlayerData, std::shared_ptr<::TroubleBrewing::PlayerCommunication>>>& playerDatas);

}


#endif //BBP_CHARACTERCHOOSER_HPP

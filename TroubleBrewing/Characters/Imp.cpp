#include "Imp.hpp"
#include "../Random.hpp"
#include <ranges>

namespace TroubleBrewing
{

void Imp::NightAction(bool zerothNight, GameState *gameState)
{
	if (zerothNight)
		return;

	Player* target = player->Communication()->InputPlayer(gameState, player,
			GetCharacterNameString() + ": Choose who you want to kill");

	// i.e. if not Lunatic or poisoned
	if (!gameState->AbilityMalfunctions(player))
	{
		target->AttemptKill(gameState, false, true, player);
	}
}

void Imp::OnDeath(GameState *gameState, bool isExecutionKill, bool isDemonKill, Player *sourcePlayer)
{
	// TODO: Scarlet Woman must always be chosen if they exist

	if (isDemonKill && sourcePlayer == player)
	{
		// Don't check perceived traits or a recluse might become the demon
		auto players = gameState->GetPlayers();
		std::vector<Player*> minions;
		std::ranges::copy(players | std::views::filter([](Player* p){
									return p->GetCharacter()->GetCharacterTraits().isMinion && !p->IsDead(); }),
						std::back_inserter(minions));

		if (minions.size() > 0) // They may have killed themself without any minion alive
		{
			Player* targetMinion = minions.at(RandomBetween(0, minions.size() - 1));
			targetMinion->SwitchCharacter(CharacterType::IMP, gameState);
		}
	}
}

}
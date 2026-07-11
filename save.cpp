#include "save.h"
#include <fstream>

Player loadPlayer()
{
	Player player;

	std::ifstream load("deposit.save", std::ios::binary);
	if (load.is_open())
	{
		load.read(reinterpret_cast<char*>(&player.deposit),			sizeof(player.deposit));
		load.read(reinterpret_cast<char*>(&player.total_score),		sizeof(player.total_score));
		load.read(reinterpret_cast<char*>(&player.total_bets_sum),	sizeof(player.total_bets_sum));
		load.read(reinterpret_cast<char*>(&player.total_bets_qty),	sizeof(player.total_bets_qty));

	}

	return player;
}

void savePlayer(const Player* player)
{
	std::ofstream save("deposit.save", std::ios::binary);
	if (save.is_open())
	{
		save.write(reinterpret_cast<const char*>(&player->deposit),			sizeof(player->deposit));
		save.write(reinterpret_cast<const char*>(&player->total_score),		sizeof(player->total_score));
		save.write(reinterpret_cast<const char*>(&player->total_bets_sum),	sizeof(player->total_bets_sum));
		save.write(reinterpret_cast<const char*>(&player->total_bets_qty),	sizeof(player->total_bets_qty));
	}
}
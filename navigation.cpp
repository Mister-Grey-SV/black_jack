#include "navigation.h"
#include "input.h"
#include "save.h"
#include "game.h"

bool exitGame()
{
	// start visual
	clearConsole();
	printConfirm();
	// end visual

	return static_cast<bool>(getNum(static_cast<int>(false), static_cast<int>(true)));
}

void setBoxesQty(Player* player)
{
	// start visual
	clearConsole();
	printWideLine();
	printPlayerInfo(player);
	printWideLine();
	printPlayerSettingsInfo(player);
	printWideLine();
	printBoxesAsk();
	// end visual

	player->box_qty = getNum(parameters::min_boxes_qty, parameters::max_boxes_qty);
}

void setBetValue(Player* player)
{
	while (true)
	{
		// start visual 
		clearConsole();
		printWideLine();
		printPlayerInfo(player);
		printWideLine();
		printPlayerSettingsInfo(player);
		printWideLine();
		printBetSizeAsk();
		// end visual

		int bet_value = getNum(parameters::min_bet,
			((player->deposit / player->box_qty < parameters::max_bet) ?
				(player->deposit / player->box_qty) : parameters::max_bet));

		if (bet_value % parameters::bet_step != 0)
		{
			continue;
		}

		player->bet_value = bet_value;
		return;
	}
}

void topUpDeposit(Player* player)
{
	// start visual
	clearConsole();
	printWideLine();
	printPlayerInfo(player);
	printWideLine();
	printDepositAsk();
	// end visual 

	player->deposit += getNum(0, parameters::max_up_value);
	savePlayer(player);
}

void tableMenu(Player* player, Player* dealer, Shoe* shoe)
{
	while (true)
	{
		// start visual
		clearConsole();
		printWideLine();
		printPlayerInfo(player);
		printWideLine();
		printPlayerSettingsInfo(player);
		printWideLine();
		printTableMenu();
		// end visual

		switch (static_cast<TableMenu>(getNum(0, parameters::table_menu_size - 1)))
		{
		case TableMenu::Exit:											return;
		case TableMenu::Draw:		playGame(player, dealer, shoe);		break;
		case TableMenu::BoxesQty:	setBoxesQty(player);				break;
		case TableMenu::Bet:		setBetValue(player);				break;
		}
	}
}
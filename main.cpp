#include "constants.h"
#include "visual.h"
#include "shuffling.h"
#include "input.h"
#include "navigation.h"
#include "save.h"
#include "game.h"

#include <iostream>
#include <array>
#include <vector>
#include <utility>
#include <random>


int main()
{
	Player player{ loadPlayer() };
	Player dealer;

	getAverageBet(&player);

	Shoe shoe{ makeShoe() };
	shoe = shuffleShoe(&shoe);

	while (true)
	{
		// start of visual
		clearConsole();
		printWideLine();
		printPlayerInfo(&player);
		printWideLine();
		printMainMenu();
		// end if visual

		switch (static_cast<MainMenu>(getNum(0, parameters::main_menu_size - 1)))
		{
			case MainMenu::Exit:		if (exitGame())						{ return 0; }	break;
			case MainMenu::Play:		tableMenu(&player, &dealer, &shoe);					break;
			case MainMenu::Deposit:		topUpDeposit(&player);								break;
		}
	}
}
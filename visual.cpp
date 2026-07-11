#include "visual.h"
#include <iostream>

void clearConsole()
{
	std::cout << "\033[2J\033[H";
}

void printWideLine()
{
	std::cout << "========================================================================================================\n";
}

void printLine()
{
	std::cout << "________________________________________________________________________________________________________\n";
}

void printLowDeposit()
{
	std::cout << "\n\n\n";
	std::cout << "\t\t\t ========[ Your deposit is not enough to play ]========\n\n";
}

void printKeyAsk()
{
	std::cout << "\t\t\t   ========[ Input any key and press Enter ]========\n";
}

void printConfirm()
{
	std::cout << "\n\n\n\n\n\n";
	std::cout << "\t\t\t\t   ========[ Are you sure? ]========\n\n";
	std::cout << "\t\t\t     ========[ [1] YES ]========[ [0] NO ]========\n";
}

void printBoxesAsk()
{
	std::cout << "\n\n\n";
	std::cout << "\t\t\t   ========[ Input boxes quentity [ " 
		<< parameters::min_boxes_qty 
		<< " - " 
		<< parameters::max_boxes_qty 
		<< " ] ]========\n";
}

void printBetSizeAsk()
{
	std::cout << "\n\n\n";
	std::cout << "\t\t   ========[ Input your bet size [ " 
		<< parameters::min_bet 
		<< " - " 
		<< parameters::max_bet 
		<< " ] step[" 
		<< parameters::bet_step 
		<< "] ]========\n";
}

void printDepositAsk()
{
	std::cout << "\n\n\n\n\n";
	std::cout << "\t\t     ========[ Input amount to deposit max [ " 
		<< parameters::max_up_value 
		<< " ] ]========\n";
}

void printInsuranceAsk(const Player* player)
{
	std::cout << "\n\n\n\n\n";
	std::cout << "\t\t     ========[ Input insuarse bet [ " 
		<< parameters::min_insurance 
		<< " - " 
		<< player->bet_value / 2 
		<< " ] step[" 
		<< parameters::step_insurance 
		<< "] ]========\n";
}

void printPlayerInfo(const Player* player)
{
	std::cout << "\t========[ DEPOSIT "
		<< player->deposit
		<< " ]========[ AVERAGE BET "
		<< player->average_bet
		<< " ]========[ TOTAL SCORE "
		<< player->total_score << "]========\n";
}

void printPlayerSettingsInfo(const Player* player)
{
	std::cout << "\t\t\t     ========[ BOXES "
		<< player->box_qty << " ]========[ BET "
		<< player->bet_value << " ]========\n";
}

void printInsurance(const Player* player, const Player* dealer)
{
	if (player->insurance_bet != 0)
	{
		std::cout << '\n';

		if (dealer->boxes[0].cards.size() >= parameters::min_cards)
		{
			if (dealer->boxes[0].condition & static_cast<uint16_t>(Condition::BlackJack))
			{
				std::cout << "   [+]  ";
			}

			else
			{
				std::cout << "   [-]  ";
			}
		}

		else
		{
			std::cout << '\t';
		}

		std::cout << "===[ INSURANCE [" << player->insurance_bet << "] ]========\n";
		printLine();
	}
}

void printCard(const Card* card)
{
	std::cout << "[ " 
		<< rank[static_cast<int>(card->rank)].name 
		<< ' '
		<< suit[static_cast<int>(card->suit)].name 
		<< " ]";
}

void printHand(const Hand* hand)
{
	std::cout << "===[ SCORE [" << hand->score << "] ]========[ BET [" << hand->bet << "] ]===";

	if (hand->condition & static_cast<uint16_t>(Condition::HasInsurance))
	{
		std::cout << "[ Insuranse ]===";
	}

	if (hand->condition & static_cast<uint16_t>(Condition::BlackJack))
	{
		std::cout << "[ BlackJack ]===";
	}

	else if (hand->condition & static_cast<uint16_t>(Condition::Splitted))
	{
		std::cout << "[ Split ]===";
	}

	if (hand->condition & static_cast<uint16_t>(Condition::Doubled))
	{
		std::cout << "[ Double ]===";
	}

	if (hand->condition & static_cast<uint16_t>(Condition::Surrender))
	{
		std::cout << "[ Surrender ]===";
	}

	if (hand->condition & static_cast<uint16_t>(Condition::EvenMoney))
	{
		std::cout << "[ Even Money ]===";
	}
	if (hand->condition & static_cast<uint16_t>(Condition::Burn))
	{
		std::cout << "[ Bust ]===";
	}

	else if (hand->condition & static_cast<uint16_t>(Condition::Stay))
	{
		std::cout << "[ Stay ]===";
	}

	std::cout << "\n\n\t";

	for (int i = 0; i < hand->cards.size(); ++i)
	{
		if (i % 5 == 0 && i != 0)
		{
			std::cout << "\n\t";
		}

		printCard(&hand->cards[i]);
		std::cout << ' ';
	}

	std::cout << "\n";
	printLine();
	std::cout << "\n";
}

void printPlayer(const Player* player, const int active)
{
	std::cout << '\n';

	for (int box = 0; box < player->boxes.size(); ++box)
	{
		if (player->boxes[box].condition & static_cast<uint16_t>(Condition::Win))
		{
			std::cout << "   [+]  ";
		}

		else if (player->boxes[box].condition & static_cast<uint16_t>(Condition::Lose))
		{
			std::cout << "   [-]  ";
		}

		else if (player->boxes[box].condition & static_cast<uint16_t>(Condition::Surrender))
		{
			std::cout << "   [s]  ";
		}

		else if (player->boxes[box].condition & static_cast<uint16_t>(Condition::StandOf))
		{
			std::cout << "   [0]  ";
		}

		else if (box == active)
		{
			std::cout << "   -->  ";
		}

		else
		{
			std::cout << '\t';
		}

		printHand(&player->boxes[box]);
	}
}

void printDealer(const Player* dealer)
{
	printWideLine();

	std::cout << "\n\t\t";
	std::cout << "========[ DEALER SCORE [" << dealer->boxes[0].score << "] ]========";

	if (dealer->boxes[0].condition & static_cast<uint16_t>(Condition::Burn))
	{
		std::cout << "[ Bust ]========";
	}

	else if (dealer->boxes[0].condition & static_cast<uint16_t>(Condition::BlackJack))
	{
		std::cout << "[ BlackJack ]========";
	}

	std::cout << "\n\n\t";

	for (int i = 0; i < dealer->boxes[0].cards.size(); ++i)
	{
		if (i % 5 == 0 && i != 0)
		{
			std::cout << "\n\t";
		}

		printCard(&dealer->boxes[0].cards[i]);
		std::cout << ' ';
	}

	std::cout << "\n\n";

	printWideLine();
}

void printMainMenu()
{
	std::cout << "\n\n\n\n\n";
	std::cout << "\t     ========";

	for (int i = 0; i < parameters::main_menu_size; ++i)
	{
		std::cout << "[ [" << static_cast<int>(main_menu[i].action) << "] " << main_menu[i].name << " ]========";
	}

	std::cout << '\n';
}

void printTableMenu()
{
	std::cout << "\n\n\n";
	std::cout << "\t     ======";

	for (int i = 0; i < parameters::table_menu_size; ++i)
	{
		std::cout << "[ [" << static_cast<int>(table_menu[i].action) << "] " << table_menu[i].name << " ]======";
	}

	std::cout << "=\n";
}

void printPlayMenu()
{
	std::cout << "\t========";

	for (int i = 0; i < parameters::play_menu_size; ++i)
	{
		if (i % 4 == 0 && i != 0)
		{
			std::cout << "\n\n\t   ========";
		}

		std::cout << "[ [" << static_cast<int>(play_menu[i].action) << "] " << play_menu[i].name << " ]========";
	}

	std::cout << '\n';
}

void printResult(const Player* player)
{
	std::cout << '\n';

	std::cout << "\t\t\t\t  ========[ Your result [" << player->round_score - player->round_total_bet << "] ]========";

	std::cout << "\n\n";

	if (player->round_score - player->round_total_bet > 0)
	{
		std::cout << "\t\t\t\t  ========[ CONGRADULATION! ]========\n\n";
	}

	else
	{
		std::cout << "\t\t\t\t========[ GOOD LUCK NEXT TIME! ]========\n\n";
	}
}

#include "game.h"
#include "input.h"
#include "shuffling.h"
#include "save.h"
#include "visual.h"
#include "navigation.h"

void getAverageBet(Player* player)
{
	player->average_bet = static_cast<long double>(player->total_bets_sum) / static_cast<long double>(player->total_bets_qty);
}

Card drawCard(Shoe* shoe)
{
	Card card = shoe->cards[shoe->current_card];
	++(shoe->current_card);

	return card;
}

int getValue(const Card* card)
{
	return card_value[static_cast<int>(card->rank)].value;
}

void updateHand(Hand* hand)
{
	int aces{};
	hand->score = 0;

	for (const Card& card : hand->cards)
	{
		hand->score += getValue(&card);
		if (card.rank == Rank::Ace)
		{
			++aces;
		}
	}

	while (hand->score > parameters::max_score && aces > 0)
	{
		hand->score -= parameters::low_high_ace_difference;
		--aces;
	}
}

void startGame(Player* player, Player* dealer, Shoe* shoe)
{
	dealer->deposit = parameters::dealer_float;

	if (shoe->current_card > shoe->cut_for_game)
	{
		*shoe = shuffleShoe(shoe);
	}

	for (int box = 0; box < player->box_qty; ++box)
	{
		player->boxes.push_back(Hand{});
		player->deposit -= player->bet_value;
		player->boxes[box].bet = player->bet_value;
		player->boxes[box].cards.push_back(drawCard(shoe));
	}

	dealer->boxes.push_back(Hand{});
	dealer->boxes[0].cards.push_back(drawCard(shoe));
	updateHand(&(dealer->boxes[0]));

	for (int box = 0; box < player->box_qty; ++box)
	{
		player->boxes[box].cards.push_back(drawCard(shoe));
		updateHand(&(player->boxes[box]));
	}
}

//playerPlays
bool updateAndChechOut(Player* player, const Player* dealer, const int box)
{
	updateHand(&(player->boxes[box]));

	if (dealer->boxes[0].cards[0].rank == Rank::Ace &&
		!(player->boxes[box].condition & static_cast<uint16_t>(Condition::Splitted)) &&
		player->boxes[box].cards.size() == parameters::min_cards &&
		player->boxes[box].score != parameters::max_score)
	{
		player->boxes[box].condition |= static_cast<uint16_t>(Condition::CanInsurance);
	}

	if (player->boxes[box].score >= parameters::max_score ||
		(player->boxes[box].condition & static_cast<uint16_t>(Condition::Doubled)) ||
		(player->boxes[box].condition & static_cast<uint16_t>(Condition::Stay)) ||
		(player->boxes[box].condition & static_cast<uint16_t>(Condition::Surrender)))
	{
		if (player->boxes[box].cards.size() == parameters::min_cards &&
			player->boxes[box].score == parameters::max_score &&
			!(player->boxes[box].condition & static_cast<uint16_t>(Condition::Splitted)) &&
			!(player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack)))
		{
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::BlackJack);
		}

		else if (player->boxes[box].score > parameters::max_score)
		{
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::Burn);
		}

		if (!(dealer->boxes[0].cards[0].rank == Rank::Ace &&
			(player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack))) ||
			(player->boxes[box].condition & static_cast<uint16_t>(Condition::ConfirmedBJ)))
		{
			return true;
		}
	}

	if (player->boxes[box].cards.size() == parameters::min_cards &&
		!(player->boxes[box].condition & static_cast<uint16_t>(Condition::Splitted)) &&
		player->boxes[box].cards[0].rank == player->boxes[box].cards[1].rank)
	{
		player->boxes[box].condition |= static_cast<uint16_t>(Condition::CanSplit);
	}

	if (player->boxes[box].cards.size() == parameters::min_cards &&
		!(player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack)) &&
		dealer->boxes[0].cards[0].rank != Rank::Ace)
	{
		player->boxes[box].condition |= static_cast<uint16_t>(Condition::CanSurrender);
	}

	if ((player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack)) &&
		dealer->boxes[0].cards[0].rank == Rank::Ace)
	{
		player->boxes[box].condition |= static_cast<uint16_t>(Condition::CanEvenMoney);
	}

	return false;
}

// playerPlays
void playerAction(Player* player, Shoe* shoe, const int box)
{
	switch (static_cast<PlayMenu>(getNum(0, parameters::play_menu_size - 1)))
	{
	case PlayMenu::Stay:
		player->boxes[box].condition |= static_cast<uint16_t>(Condition::Stay);

		if (player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack))
		{
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::ConfirmedBJ);
		}

		return;

	case PlayMenu::EvenMoney:
		if (player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack))
		{
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::EvenMoney);
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::ConfirmedBJ);
		}

		return;

	case PlayMenu::Surrender:
		if (player->boxes[box].condition & static_cast<uint16_t>(Condition::CanSurrender))
		{
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::Surrender);
			return;
		}

		return;

	case PlayMenu::Card:
		if (!(player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack)))
		{
			player->boxes[box].cards.push_back(drawCard(shoe));
		}

		return;

	case PlayMenu::Double:
		if (player->deposit < player->boxes[box].bet ||
			(player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack)))
		{
			return;
		}

		player->deposit -= player->boxes[box].bet;
		player->boxes[box].bet *= 2;
		player->boxes[box].condition |= static_cast<uint16_t>(Condition::Doubled);
		player->boxes[box].cards.push_back(drawCard(shoe));
		return;

	case PlayMenu::Split:
		if ((player->boxes[box].condition & static_cast<uint16_t>(Condition::CanSplit)) &&
			player->deposit >= player->boxes[box].bet)
		{
			player->deposit -= player->boxes[box].bet;
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::Splitted);
			player->boxes[box].condition &= ~static_cast<uint16_t>(Condition::CanSplit);

			player->boxes.insert(player->boxes.begin() + box + 1, Hand{});
			player->boxes[box + 1].bet = player->boxes[box].bet;
			player->boxes[box + 1].condition |= static_cast<uint16_t>(Condition::Splitted);

			player->boxes[box + 1].cards.push_back(player->boxes[box].cards[1]);
			player->boxes[box].cards.erase(player->boxes[box].cards.begin() + 1);

			player->boxes[box].cards.push_back(drawCard(shoe));
			player->boxes[box + 1].cards.push_back(drawCard(shoe));
			return;
		}

		return;

	case PlayMenu::Insurance:
		if ((player->boxes[box].condition & static_cast<uint16_t>(Condition::CanInsurance)) &&
			!(player->boxes[box].condition & static_cast<uint16_t>(Condition::HasInsurance)))
		{
			if (player->deposit >= player->boxes[box].bet / 2)
			{
				int insurance{};
				while (true)
				{
					// start visual
					clearConsole();
					printWideLine();
					printPlayerInfo(player);
					printWideLine();
					printInsuranceAsk(player);
					// end visual

					insurance = getNum(parameters::min_insurance, player->boxes[box].bet / 2);
					if (insurance % 5 == 0)
					{
						break;
					}
				}

				player->boxes[box].condition |= static_cast<uint16_t>(Condition::HasInsurance);
				player->deposit -= insurance;
				player->insurance_bet += insurance;
			}
		}

		return;
	}
}

void dealerPlays(Player* dealer, Shoe* shoe)
{
	dealer->boxes[0].cards.push_back(drawCard(shoe));

	while (true)
	{
		updateHand(&(dealer->boxes[0]));

		if (dealer->boxes[0].score >= parameters::max_score ||
			(dealer->boxes[0].condition & static_cast<uint16_t>(Condition::Stay)))
		{
			if (dealer->boxes[0].cards.size() == parameters::min_cards &&
				dealer->boxes[0].score == parameters::max_score)
			{
				dealer->boxes[0].condition |= static_cast<uint16_t>(Condition::BlackJack);
			}

			else if (dealer->boxes[0].score > parameters::max_score)
			{
				dealer->boxes[0].condition |= static_cast<uint16_t>(Condition::Burn);
			}

			break;
		}

		if (dealer->boxes[0].score < parameters::dealer_max_draw)
		{
			dealer->boxes[0].cards.push_back(drawCard(shoe));
		}

		else
		{
			dealer->boxes[0].condition |= static_cast<uint16_t>(Condition::Stay);
		}
	}
}

void checkWinners(Player* player, Player* dealer)
{
	player->round_total_bet += player->insurance_bet;

	if (dealer->boxes[0].condition & static_cast<uint16_t>(Condition::BlackJack))
	{
		player->round_score += player->insurance_bet * 2;
	}

	else
	{
		dealer->round_score += player->insurance_bet;
	}

	for (int box = 0; box < player->boxes.size(); ++box)
	{
		player->round_total_bet += player->boxes[box].bet;

		if (player->boxes[box].condition & static_cast<uint16_t>(Condition::Burn)) // lose
		{
			dealer->round_score += player->boxes[box].bet;
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::Lose);

			continue;
		}

		else if (player->boxes[box].condition & static_cast<uint16_t>(Condition::Surrender)) // lose 0.5
		{
			player->round_score += player->boxes[box].bet / 2;
			dealer->round_score += player->boxes[box].bet / 2;

			continue;
		}

		else if (player->boxes[box].condition & static_cast<uint16_t>(Condition::EvenMoney)) // win
		{
			player->round_score += player->boxes[box].bet * 2;
			dealer->round_score -= player->boxes[box].bet;
			player->boxes[box].condition |= static_cast<uint16_t>(Condition::Win);

			continue;
		}

		else if (player->boxes[box].condition & static_cast<uint16_t>(Condition::BlackJack))
		{
			if (dealer->boxes[0].condition & static_cast<uint16_t>(Condition::BlackJack)) // stand of
			{
				player->round_score += player->boxes[box].bet;
				player->boxes[box].condition |= static_cast<uint16_t>(Condition::StandOf);

				continue;
			}

			else // win 1.5
			{
				player->round_score += player->boxes[box].bet * 2 + player->boxes[box].bet / 2;
				dealer->round_score -= player->boxes[box].bet + player->boxes[box].bet / 2;
				player->boxes[box].condition |= static_cast<uint16_t>(Condition::Win);

				continue;
			}
		}

		else
		{
			if (player->boxes[box].score < dealer->boxes[0].score &&
				!(dealer->boxes[0].condition & static_cast<uint16_t>(Condition::Burn))) // lose
			{
				dealer->round_score += player->boxes[box].bet;
				player->boxes[box].condition |= static_cast<uint16_t>(Condition::Lose);

				continue;
			}

			else if (player->boxes[box].score == dealer->boxes[0].score) // stand of
			{
				player->round_score += player->boxes[box].bet;
				player->boxes[box].condition |= static_cast<uint16_t>(Condition::StandOf);

				continue;
			}

			else // win
			{
				player->round_score += player->boxes[box].bet * 2;
				dealer->round_score -= player->boxes[box].bet;
				player->boxes[box].condition |= static_cast<uint16_t>(Condition::Win);
			}
		}
	}
}

void writeResult(Player* player, Player* dealer)
{
	player->total_score += player->round_score - player->round_total_bet;
	player->total_bets_sum += player->round_total_bet;
	player->total_bets_qty += player->boxes.size();

	player->deposit += player->round_score;
	dealer->deposit += dealer->round_score;
}

void cleanTable(Player* player, Player* dealer)
{
	player->round_total_bet = 0;
	player->round_score = 0;
	player->insurance_bet = 0;
	if (!player->boxes.empty())
	{
		player->boxes.clear();
	}

	dealer->round_score = 0;
	if (!dealer->boxes.empty())
	{
		dealer->boxes.clear();
	}
}

void playGame(Player* player, Player* dealer, Shoe* shoe)
{
	if (player->deposit < player->bet_value * player->box_qty) // checking bets
	{
		// start visual
		clearConsole();
		printWideLine();
		printPlayerInfo(player);
		printWideLine();
		printLowDeposit();
		printKeyAsk();
		// end visual

		getKey();

		return;
	}

	startGame(player, dealer, shoe);

	for (int box = 0; box < player->boxes.size(); ++box)
	{
		while (true)
		{
			if (updateAndChechOut(player, dealer, box))
			{
				break;
			}

			// start visual
			clearConsole();
			printWideLine();
			printPlayerInfo(player);
			printWideLine();
			printDealer(dealer);
			printInsurance(player, dealer);
			printPlayer(player, box);
			printPlayMenu();
			// end visual

			playerAction(player, shoe, box);
		}
	}

	dealerPlays(dealer, shoe);
	checkWinners(player, dealer);
	writeResult(player, dealer);

	// start visual
	clearConsole();
	printWideLine();
	printPlayerInfo(player);
	printWideLine();
	printDealer(dealer);
	printInsurance(player, dealer);
	printPlayer(player, parameters::out_of_base_value);
	printResult(player);
	// end visual

	cleanTable(player, dealer);

	// save player
	savePlayer(player);
	getAverageBet(player);

	//message "Press Enter"
	printKeyAsk();
	getKey();
}
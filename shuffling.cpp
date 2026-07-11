#include "shuffling.h"
#include <random>


// making
type::Deck makeDeck()	// making and filling the deck
{
	type::Deck deck{};

	int suit = 0;
	int rank = 0;
	for (Card& card : deck)
	{
		if (rank == parameters::rank_size)
		{
			rank = 0;
			++suit;
		}

		card.rank = static_cast<Rank>(rank);
		card.suit = static_cast<Suit>(suit);
		++rank;
	}

	return deck;
}

Shoe makeShoe() // making and filling the shoe
{
	Shoe shoe{};

	for (int deck = 0; deck < parameters::decks_qty; ++deck)
	{
		type::Deck current_deck = makeDeck();
		shuffleDeck(&current_deck);

		for (int card = 0; card < parameters::deck_size; ++card)
		{
			shoe.cards[deck * parameters::deck_size + card] = current_deck[card];
		}
	}

	return shoe;
}


// shuffling
int getRandom(const int num1, const int num2, const Random type)	// getting random position in the deck
{
	static std::mt19937 generator{ std::random_device{}() };

	if (type == Random::Uniform)
	{
		std::uniform_int_distribution<int> distribution(num1, num2);

		return distribution(generator);
	}

	else if (type == Random::Normal)
	{
		int result{};

		while (result < num1 - num2 * 2 ||
			num1 + num2 * 2 < result ||
			result >= parameters::deck_size * parameters::decks_qty)
		{
			std::normal_distribution<double> distribution(num1, num2);
			result = static_cast<int>(distribution(generator));
		}

		return result;
	}
}

void shuffleDeck(type::Deck* deck) // shuffling the deck
{
	for (int card = 0; card < parameters::deck_size; ++card)
	{
		std::swap(deck->at(card), deck->at(getRandom(1, deck->size() - 1, Random::Uniform)));
	}
}

Shoe shuffleShoe( Shoe* shoe) // shuffling the shoe
{
	for (int time = 0; time < parameters::shuffle_times; ++time)
	{
		for (int card = 0; card < shoe->cards.size(); ++card)
		{
			std::swap(shoe->cards[card], shoe->cards[getRandom(1, shoe->cards.size() - 1, Random::Uniform)]);
		}
	}

	int check{ getRandom(parameters::cut_for_check, parameters::deviation, Random::Normal) };
	Shoe newshoe{};

	for (int i = check, j = 0; i < shoe->cards.size(); ++i, ++j)
	{
		newshoe.cards[j] = shoe->cards[i];
	}

	for (int i = 0, j = shoe->cards.size() - check; i < check; ++i, ++j)
	{
		newshoe.cards[j] = shoe->cards[i];
	}

	newshoe.current_card = 0;
	newshoe.cut_for_game = getRandom(parameters::cut_for_game, parameters::deviation, Random::Normal);

	return newshoe;
}

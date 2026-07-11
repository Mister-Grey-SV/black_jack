#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>
#include <array>

// deck
enum class Suit
{
	Hearts,
	Clubs,
	Diamond,
	Spades
};

enum class Rank
{
	Ace,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	Ten,
	Jack,
	Queen,
	King
};

struct CardValue
{
	Rank rank;
	const int value;
};

inline constexpr CardValue card_value[]
{
	{	Rank::Ace,		11	},
	{	Rank::Two,		2	},
	{	Rank::Three,	3	},
	{	Rank::Four,		4	},
	{	Rank::Five,		5	},
	{	Rank::Six,		6	},
	{	Rank::Seven,	7	},
	{	Rank::Eight,	8	},
	{	Rank::Nine,		9	},
	{	Rank::Ten,		10	},
	{	Rank::Jack,		10	},
	{	Rank::Queen,	10	},
	{	Rank::King,		10	},
};

struct Card
{
	Suit suit;
	Rank rank;
};

struct StructRank
{
	Rank rank;
	const char* name;
};

inline constexpr StructRank rank[]
{
	{	Rank::Ace,		"Ace"	},
	{	Rank::Two,		"2"		},
	{	Rank::Three,	"3"		},
	{	Rank::Four,		"4"		},
	{	Rank::Five,		"5"		},
	{	Rank::Six,		"6"		},
	{	Rank::Seven,	"7"		},
	{	Rank::Eight,	"8"		},
	{	Rank::Nine,		"9"		},
	{	Rank::Ten,		"10"	},
	{	Rank::Jack,		"Jack"	},
	{	Rank::Queen,	"Queen"	},
	{	Rank::King,		"King"	}
};

struct StructSuit
{
	Suit suit;
	const char* name;
};

inline constexpr StructSuit suit[]
{
	{	Suit::Hearts,	"Hearts"	},
	{	Suit::Clubs,	"Clubs"		},
	{	Suit::Diamond,	"Diamond"	},
	{	Suit::Spades,	"Spades"	}
};

// navigation
enum class MainMenu
{
	Exit,
	Play,
	Deposit
};

struct StructMainMenu
{
	MainMenu action;
	const char* name;
};

inline constexpr StructMainMenu main_menu[]
{
	{	MainMenu::Exit,		"Exit"				},
	{	MainMenu::Play,		"Table"				},
	{	MainMenu::Deposit,	"Add to depotis"	},
};

enum class TableMenu
{
	Exit,
	Draw,
	BoxesQty,
	Bet
};

struct StructTableMenu
{
	TableMenu action;
	const char* name;
};

inline constexpr StructTableMenu table_menu[]
{
	{	TableMenu::Exit,		"Back"	},
	{	TableMenu::Draw,		"Play"	},
	{	TableMenu::BoxesQty,	"Boxes"	},
	{	TableMenu::Bet,			"Bet"	}
};

enum class PlayMenu
{
	Stay,
	Card,
	Split,
	Double,
	Surrender,
	EvenMoney,
	Insurance
};

struct StructPlayMenu
{
	PlayMenu action;
	const char* name;
};

inline constexpr StructPlayMenu play_menu[]
{
	{	PlayMenu::Stay,			"Stay"			},
	{	PlayMenu::Card,			"Card"			},
	{	PlayMenu::Split,		"Split"			},
	{	PlayMenu::Double,		"Double"		},
	{	PlayMenu::Surrender,	"Surrender"		},
	{	PlayMenu::EvenMoney,	"Even Money"	},
	{	PlayMenu::Insurance,	"Insurance"		}
};

// parameters
namespace parameters
{
	inline constexpr int		deck_size				{ 52 };
	inline constexpr int		decks_qty				{ 6 };

	inline constexpr int		shuffle_times			{ 3 };
	inline constexpr int		cut_for_check			{ 156 };
	inline constexpr int		cut_for_game			{ 207 };
	inline constexpr int		deviation				{ 13 };

	inline constexpr int		min_boxes_qty			{ 1 };
	inline constexpr int		max_boxes_qty			{ 7 };

	inline constexpr int		min_cards				{ 2 };
	inline constexpr int		max_score				{ 21 };
	inline constexpr int		dealer_max_draw			{ 17 };
	inline constexpr int		low_high_ace_difference	{ 10 };

	inline constexpr int		min_bet					{ 10 };
	inline constexpr int		max_bet					{ 1000 };
	inline constexpr int		bet_step				{ 10 };
	inline constexpr int		min_insurance			{ 5 };
	inline constexpr int		step_insurance			{ 5 };

	inline constexpr long long	dealer_float			{ 1000000000000 };
	inline constexpr int		max_up_value			{ 10000000 };
	inline constexpr int		out_of_base_value		{ 1000 };

	inline constexpr int		rank_size		{ sizeof(rank)			/	sizeof(rank[0]) };
	inline constexpr int		main_menu_size	{ sizeof(main_menu)		/	sizeof(main_menu[0]) };
	inline constexpr int		table_menu_size	{ sizeof(table_menu)	/	sizeof(table_menu[0]) };
	inline constexpr int		play_menu_size	{ sizeof(play_menu)		/	sizeof(play_menu[0]) };
}

namespace type
{
	using Deck = std::array<Card, parameters::deck_size>;
	using Shoe = std::array<Card, parameters::decks_qty * parameters::deck_size>;
}

// player
enum class Condition : uint16_t
{
	Doubled			= 1 << 0,
	Splitted		= 1 << 1,
	BlackJack		= 1 << 2,
	Burn			= 1 << 3,
	Surrender		= 1 << 4,
	EvenMoney		= 1 << 5,
	Stay			= 1 << 6,
	CanSplit		= 1 << 7,
	CanSurrender	= 1 << 8,
	CanEvenMoney	= 1 << 9,
	ConfirmedBJ		= 1 << 10,
	CanInsurance	= 1 << 11,
	HasInsurance	= 1 << 12,
	Win				= 1 << 13,
	Lose			= 1 << 14,
	StandOf			= 1 << 15
};

struct Hand
{
	uint16_t condition{};
	std::vector<Card> cards;
	int score{};
	int bet{};
};

struct Player
{
	int box_qty{ parameters::min_boxes_qty };
	int bet_value{ parameters::min_bet };
	std::vector<Hand> boxes;
	long long deposit{};
	
	long long total_bets_sum{};
	long long total_bets_qty{};
	long long total_score{};
	long double average_bet{};

	int insurance_bet{};

	int round_total_bet{};
	int round_score{};
};

// shoe
enum class Random
{
	Uniform,
	Normal
};

struct Shoe
{
	type::Shoe cards{};
	int current_card{};
	int cut_for_game{};
};

#endif // !CONSTANTS_H


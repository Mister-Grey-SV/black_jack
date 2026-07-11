#ifndef SHUFFLING_H
#define SHUFFLING_H

#include "constants.h"

//	making
type::Deck makeDeck();	// making and filling the deck
Shoe makeShoe();		// making and filling the shoe

//	shuffing
int getRandom(const int num1, const int num2, const Random type);	// getting random position in the deck 
void shuffleDeck(type::Deck* deck); // shuffling the deck
Shoe shuffleShoe(Shoe* shoe); // shuffling the shoe

#endif // !SHUFFLING_H

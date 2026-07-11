#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "input.h"

void getAverageBet(Player* player);

void playGame(Player* player, Player* dealer, Shoe* shoe);

void startGame(Player* player, Player* dealer, Shoe* shoe);
bool updateAndChechOut(Player* player, const Player* dealer, const int box);
void playerAction(Player* player, Shoe* shoe, const int box);
void dealerPlays(Player* dealer, Shoe* shoe);
void checkWinners(Player* player, Player* dealer);
void writeResult(Player* player, Player* dealer);
void cleanTable(Player* player, Player* dealer);

Card drawCard(Shoe* shoe);
int getValue(const Card* card);
void updateHand(Hand* hand);

#endif // !GAME_H

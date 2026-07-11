#ifndef NAVIGATION_H
#define NAVIGATION_H
#include "constants.h"
#include "visual.h"

bool exitGame();

void topUpDeposit(Player* player);
void setBoxesQty(Player* player);
void setBetValue(Player* player);

void tableMenu(Player* player, Player* dealer, Shoe* shoe);

#endif // !NAVIGATION_H

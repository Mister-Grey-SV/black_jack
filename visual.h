#ifndef VISUAL_H
#define VISUAL_H

#include "constants.h"

void clearConsole();
void printWideLine();
void printLine();

void printLowDeposit();

void printKeyAsk();
void printConfirm();
void printBoxesAsk();
void printBetSizeAsk();
void printDepositAsk();
void printInsuranceAsk(const Player* player);

void printCard(const Card* card);
void printHand(const Hand* hand);
void printPlayer(const Player* player, const int active);
void printDealer(const Player* dealer);

void printPlayerInfo(const Player* player);
void printPlayerSettingsInfo(const Player* player);
void printInsurance(const Player* player, const Player* dealer);

void printMainMenu();
void printTableMenu();
void printPlayMenu();

void printResult(const Player* player);

#endif // !VISUAL_H
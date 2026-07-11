#ifndef INPUT_H
#define INPUT_H

#include <string>

int getNum(const int min, const int max);
bool tryParse(const std::string& input, int& output);
void getKey();

#endif // !INPUT_H

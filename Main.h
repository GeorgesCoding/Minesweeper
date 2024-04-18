#ifndef MAIN_H
#define MAIN_H

#include <iostream>

int main();
bool game();
void start(char *&board, std::size_t &x, std::size_t &y, unsigned int &numMines);
char getAction();
void show(char *&board, std::size_t &x, std::size_t &y, unsigned int &numMines);
void actionMark(char *board, std::size_t x, std::size_t y);
std::size_t xMax();
std::size_t yMax();
char marked_mask();
char hidden_mask();
char value_mask();
void placeMines(char *board, std::size_t x, std::size_t y, unsigned int numMines);

#endif

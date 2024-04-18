#ifndef CONTROLLER
#define CONTROLLER_H

char *createBoard(std::size_t x, std::size_t y);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t x, std::size_t y);
void hideBoard(char *board, std::size_t x, std::size_t y);
int mark(char *board, std::size_t x, std::size_t y, std::size_t xLoc, std::size_t yLoc);

void computeSurrounding(char *board, std::size_t x, std::size_t y);
bool gameEnd(char *board, std::size_t x, std::size_t y);
int reveal(char *board, std::size_t x, std::size_t y, std::size_t xLoc, std::size_t yLoc);

#endif

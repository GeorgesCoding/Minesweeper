#include "Main.h"
#include <iostream>

char *createBoard(std::size_t x, std::size_t y)
{
    char *arr{new char[(x * y)]{}};

    for (std::size_t i{0}; i < (x * y); i++)
    {
        arr[i] == 0b0;
    }

    return arr;
}

void cleanBoard(char *board)
{
    delete[] board;
    board = nullptr;
}

void printBoard(char *board, std::size_t x, std::size_t y)
{
    int row{0};

    for (std::size_t i{0}; i < (x * y); i++, row++) // loops through index
    {
        if (row == x) // Goes to Next Line Once xdim is Reached
        {
            row = 0;
            std::cout << std::endl;
        }

        if ((board[i] & 0b00010000) == 0b00010000) // Mark
        {
            std::cout << "M";
        }
        else if ((board[i] & 0b00100000) == 0b00100000) // Hidden
        {
            std::cout << "*";
        }

        else if ((board[i] & 0b00100000) == 0b00000000) // Reveal
        {
            unsigned int field = (board[i] & 0b00001111);
            std::cout << field;
        }
    }
    std::cout << std::endl;
}

void hideBoard(char *board, std::size_t x, std::size_t y)
{
    for (std::size_t i{0}; i < (x * y); i++)
    {
        board[i] = board[i] | 0b00100000;
    }
}

int mark(char *board, std::size_t x, std::size_t y, std::size_t xLoc, std::size_t yLoc)
{
    std::size_t index = (x * yLoc) + xLoc; // computes index

    if ((board[index] & 0b00100000) == 0b00000000) // Already Revealed
    {
        return 2;
    }

    else if ((board[index] & 0b00010000) == 0b00010000) // Turn Off
    {
        board[index] = board[index] & ~0b00010000;
        return 0;
    }

    else if ((board[index] & 0b00010000) == 0b00000000) // Turn On
    {
        board[index] = board[index] | 0b00010000;
        return 0;
    }
    else
    {
        return 0;
    }
}

void computeSurrounding(char *board, std::size_t x, std::size_t y)
{
    for (std::size_t i{0}; i < y; i++) // row number
    {
        for (std::size_t j{0}; j < x; j++) // column number
        {
            if ((board[x * i + j]) == 9)
            {
                for (int k{0}; k < 3; k++)
                {
                    if (i - 1 >= 0 && i - 1 <= y && ((x * i + j) - x - 1 + k) / x == i - 1 && board[(x * i + j) - x - 1 + k] != 9)
                    {
                        board[(x * i + j) - x - 1 + k] += 1;
                    }
                }

                if (((x * i + j) - 1) / x == i && board[(x * i + j) - 1] != 9) // check for same row as field
                {
                    board[(x * i + j) - 1] += 1;
                }
                if (((x * i + j) + 1) / x == i && board[(x * i + j) + 1] != 9) // check for same row as field
                {
                    board[(x * i + j) + 1] += 1;
                }

                for (int l{0}; l < 3; l++)
                {
                    if (i + 1 <= y && (((x * i + j) + x - 1 + l) / x) == (i + 1) && board[(x * i + j) + x - 1 + l] != 9)
                    {
                        board[(x * i + j) + x - 1 + l] += 1;
                    }
                }
            }
        }
    }
}

int reveal(char *board, std::size_t x, std::size_t y, std::size_t xLoc, std::size_t yLoc)
{
    std::size_t index = (x * yLoc) + xLoc; // computes index
    int i = index / x;

    if ((board[index] & 0b00001001) == 0b00001001) // mines
    {
        board[index] = board[index] & ~0b00100000;
        return 9;
    }
    else if (((board[index] & 0b00010000) == 0b00010000)) // marked
    {
        return 1;
    }
    else if (((board[index] & 0b00100000) == 0b00000000)) // revealed
    {
        return 2;
    }
    else if (((board[index] & 0b00001111) != 0b00000000)) // adjacent mines
    {
        board[index] = board[index] & ~0b00100000;
        return 0;
    }
    else // zero adjacent
    {
        board[index] &= ~0b00100000;

        for (int k{0}; k < 3; k++)
        {
            if (i - 1 >= 0 && i - 1 <= y && ((index)-x - 1 + k) / x == i - 1 && (board[(index)-x - 1 + k] & 0b00001001) != 0b00001001)
            {
                board[(index)-x - 1 + k] &= ~0b00100000;
            }
        }

        if (((index)-1) / x == i && (board[(index)-1] & 0b00001001) != 0b00001001) // check for same row as field
        {
            board[(index)-1] &= ~0b00100000;
        }
        if (((index) + 1) / x == i && (board[(index) + 1] & 0b00001001) != 0b00001001) // check for same row as field
        {
            board[(index) + 1] &= ~0b00100000;
        }

        for (int l{0}; l < 3; l++)
        {
            if (i + 1 <= y && (((index) + x - 1 + l) / x) == (i + 1) && (board[(index) + x - 1 + l] & 0b00001001) != 0b00001001)
            {
                board[(index) + x - 1 + l] &= ~0b00100000;
            }
        }
        return 0;
    }
}

bool gameEnd(char *board, std::size_t x, std::size_t y)
{
    std::size_t capacity{x * y}; // capacity of arrays
    int mines{0};
    int fields{0};

    for (std::size_t i{0}; i < capacity; i++) // loops through index
    {
        if (((board[i] & 0b00001001) != 0b00001001) && ((board[i] & 0b00100000) == 0b00000000))
        {
            fields++; // number of revealed fields
        }
        else if ((board[i] & 0b00001001) == 0b00001001)
        {
            mines++; // number of mines
        }
    }

    if (mines == capacity)
    {
        return true;
    }
    else if (fields + mines == capacity)
    {
        return true;
    }
    else
    {
        return false;
    }
}
#include <iostream>
#include <cctype>
#include <ctime>
#include "Main.h"
#include "Controller.h"

int main()
{
  srand(time(NULL));
  game();
  return 0;
}

// main game function
bool game()
{
  std::size_t x{0};
  std::size_t y{0};

  unsigned int numMines{0};
  char *board{NULL};

  start(board, x, y, numMines);

  char action{0};
  while (action != 'Q')
  {
    switch (action)
    {
    case 'S': // show
    {
      show(board, x, y, numMines);
      break;
    }
    case 'M': // mark
    {
      actionMark(board, x, y);
      break;
    }
    case 'R': // restart
    {
      std::cout << "Restarting the game." << std::endl;
      start(board, x, y, numMines);
      break;
    }
    }
    printBoard(board, x, y);

    if (gameEnd(board, x, y))
    {
      std::cout << "YOU WON!!!" << std::endl;
      std::cout << "You have revealed all the fields stepping on a mine!" << std::endl;

      for (std::size_t revealRow{0}; revealRow < y; revealRow++)
      {
        for (std::size_t revealCol{0}; revealCol < x; revealCol++)
        {
          board[revealRow * x + revealCol] = (board[revealRow * x + revealCol] & value_mask());
        }
      }

      printBoard(board, x, y);
      std::cout << "Resetting the game board." << std::endl;
      start(board, x, y, numMines);
      printBoard(board, x, y);
    }

    action = getAction();
  }

  cleanBoard(board);
  return true;
}

// gets initial conditions of the board
void start(char *&board, std::size_t &x, std::size_t &y, unsigned int &numMines)
{
  std::cout << "Welcome to Minesweeper!" << std::endl;
  do
  {
    std::cout << "Please enter the x dimension (max " << xMax() << "): ";
    std::cin >> x;
  } while (x < 1 || x > xMax());

  do
  {
    std::cout << "Please enter the y dimension (max " << yMax() << "): ";
    std::cin >> y;
  } while (y < 1 || y > yMax());

  std::cout << "Please enter the number of mines: ";
  std::cin >> numMines;
  while (numMines > x * y)
  {
    std::cout << "That's too many mines!" << std::endl;
    std::cout << "Please enter the number of mines: ";
    std::cin >> numMines;
  }

  // calls functions to create the board
  cleanBoard(board);
  board = createBoard(x, y);
  placeMines(board, x, y, numMines);
  computeSurrounding(board, x, y);
  hideBoard(board, x, y);

  return;
}

char getAction()
{
  char action{0};

  std::cout << "Please enter the action ([S]how, [M]ark, [R]estart, [Q]uit): ";
  std::cin >> action;

  if (islower(action))
    action = toupper(action);

  return action;
}

void show(char *&board, std::size_t &x, std::size_t &y, unsigned int &numMines)
{
  std::size_t revealX{0};
  std::size_t revealY{0};
  std::cout << "Please enter the x location to show: ";
  std::cin >> revealX;
  std::cout << "Please enter the y location to show: ";
  std::cin >> revealY;

  if (revealX >= x || revealY >= y)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (board[x * revealY + revealX] & marked_mask())
  {
    std::cout << "Location is marked, and therefore cannot be revealed." << std::endl;
    std::cout << "Use Mark on location to unmark." << std::endl;
  }
  else if (reveal(board, x, y, revealX, revealY) == 9)
  {
    std::cout << "You stepped on a mine! Your game has ended." << std::endl;
    printBoard(board, x, y);
    std::cout << "Starting a new game." << std::endl;
    start(board, x, y, numMines);
  }

  return;
}

void actionMark(char *board, std::size_t x, std::size_t y)
{
  std::size_t x_mark{0};
  std::size_t y_mark{0};
  std::cout << "Please enter the x location to mark: ";
  std::cin >> x_mark;
  std::cout << "Please enter the y location to mark: ";
  std::cin >> y_mark;

  if (x_mark >= x || y_mark >= y)
  {
    std::cout << "Location entered is not on the board." << std::endl;
  }
  else if (mark(board, x, y, x_mark, y_mark) == 2)
  {
    std::cout << "Position already revealed, so cannot be marked." << std::endl;
  }
  return;
}

std::size_t xMax()
{
  return 60;
}

std::size_t yMax()
{
  return 20;
}

char marked_mask()
{
  return 0x10;
}

char hidden_mask()
{
  return 0x20;
}

char value_mask()
{
  return 0x0F;
}

void placeMines(char *board, std::size_t x, std::size_t y, unsigned int numMines)
{
  if (board != NULL)
  {
    for (unsigned int i{0}; i < numMines; i++)
    {
      std::size_t temp{0};
      do
      {
        temp = rand() % (x * y);
      } while (board[temp] != 0);

      board[temp] = 9;
    }
  }
}

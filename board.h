#ifndef BOARD_H
#define BOARD_H

#include <vector>

//#include "util.h"
#include "printer.h"
#include "player.h"
#include "util.h"

class Cell {
 public:
  Cell(int row, int col)
      : column_coordinate(col), row_coordinate(row), occupied(false), playerOwned(NULL) {
  }
  ~Cell() {}

  bool occupy(Player *&p);
  void free();
  Player *changePossession(Player *newPlayer);

  bool isOccupied();
  bool belongsTo(const Player *p);

  int getRow();
  int getColumn();
  Player *getPlayerOwned();

 private:
  int row_coordinate;
  int column_coordinate;

  bool occupied;
  Player *playerOwned;
};

class Board {
  friend class Printer;

 public:
  Board(int size);
  ~Board();

  bool playAt(const int row, const int col, Player *p);
  Player *removeStone(const int row, const int col);
  int countStonesFor(const Player *p);

  bool isCellOccupied(const int row, const int col);
  bool isCellOwnedBy(const int row, const int col, const Player *p);
  bool isEmpty();
  bool isFull();

  int getSize();

  void Clear();

  bool InBorder(const int &x, const int &y);

  int KeyPressed(HANDLE con, Player *&p);

  int CalcLiberties(const int &row, const int &col, Player *p);
  bool IsKilled(Player *p1, Player *p2);
  bool CheckKo(const int& row, const int& col, Player* p);

  void saveBoard(FILE*& f, bool turn, Player* p, Player* p2);
  int loadBoard(FILE*& f, bool& turn, Player*& p1, Player*& p2);

 private:
  int size;
  Cell **table;
  int occupiedCells;

  Cell &getCell(const int row, const int col);
};

#endif // BOARD_H

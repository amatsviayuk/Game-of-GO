#ifndef PRINTER_H
#define PRINTER_H

#include "board.h"
#include "player.h"
#include "util.h"

class Board;

class Printer {
 public:
  explicit Printer(char emptyCell = '.', char player1 = p1Icon, char player2 = p2Icon);
  ~Printer() = default;

  void printBoard(HANDLE con, Board &board, const Player *player1) const;
  static void printLegend(HANDLE con, Player* p1, Player* p2);
  int requestFieldSize(HANDLE con);

  char getP1Icon();
  char getP2Icon();
  char getEmptyIcon();

 private:
  char emptyCellIcon;
  char player1Icon;
  char player2Icon;
};

#endif //PRINTER_H

#ifndef UTIL_H
#define UTIL_H

#include <cstdio>
#include <cstdlib>
#include <dos.h>

enum keys {
  ENTER = 13,
  ESC = 27,
  UP = 72,
  DOWN = 80,
  LEFT = 75,
  RIGHT = 77,
  FINISH = 102,
  PLACE = 105,
  LOAD = 108,
  NEW_GAME = 110,
  SAVE = 115,
  QUIT = 113,
};

enum exit_codes {
  EXIT_DEFAULT,
  EXIT_MOVE,
  EXIT_NEW_GAME,
  EXIT_QUIT,
  EXIT_PLACE,
  EXIT_ENTER,
  EXIT_ESC,
  EXIT_FINISH,
  EXIT_SAVE,
  EXIT_LOAD
};

const int xPos = 50;  // x position of a board
const int yPos = 2;   // y position of a board
COORD cursorPos;

int cursor_x = 0;
int cursor_y = 0;

int last_inserted_x = NULL;
int last_inserted_y = NULL;

bool Ko = false;

const char emptyIcon = '.';
const char p1Icon = 'x';
const char p2Icon = 'o';

void moveCursor(const short& x, const short& y, HANDLE con) {
  cursorPos.X = x;
  cursorPos.Y = y;
  SetConsoleCursorPosition(con, cursorPos);
}

#endif // UTIL_H

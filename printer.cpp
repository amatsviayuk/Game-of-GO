#include "printer.h"

#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

Printer::Printer(char emptyCell, char player1, char player2)
    : emptyCellIcon(emptyCell), player1Icon(player1), player2Icon(player2) {}

void Printer::printBoard(HANDLE con, Board &board, const Player *player1) const {
  DWORD written;

  cursorPos.X = xPos;
  cursorPos.Y = yPos;

  const string kLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int n = board.size;

  auto letters = new TCHAR[2 * n];
  for (int i = 0; i < 2 * n; i += 2) {
    letters[i] = kLetters[i / 2];
    letters[i + 1] = ' ';
  }
  SetConsoleCursorPosition(con, cursorPos);
  WriteConsole(con, letters, 2 * n, &written, nullptr);

  cursorPos.X--;
  cursorPos.Y++;
  auto hor_border = new TCHAR[2 * n + 2];
  hor_border[0] = '+';
  memset(hor_border + sizeof(TCHAR), '-', 2 * n);
  hor_border[2 * n + 1] = '+';
  SetConsoleCursorPosition(con, cursorPos);
  WriteConsole(con, hor_border, 2 * n + 2, &written, nullptr);

  // Printing columns
  cursorPos.X -= 2;
  for (int i = 0; i < n; i++) {
    cursorPos.Y++;
    auto row = new TCHAR[2 * n + 6];
    row[0] = i >= 9 ? char((i + 1) / 10 + '0') : ' ';
    row[1] = i >= 9 ? char((i + 1) % 10 + '0') : char(i + 1 + '0');
    row[2] = '|';

    for (int j = 0; j < 2 * n; j += 2) {
      Cell c = board.getCell(i, j / 2);
      if (!c.isOccupied()) {
        row[j + 3] = this->emptyCellIcon;
      } else if (c.getPlayerOwned() == player1) {
        row[j + 3] = this->player1Icon;
      } else {
        row[j + 3] = this->player2Icon;
      }
      row[j + 4] = ' ';
    }

    row[2 * n + 3] = '|';
    row[2 * n + 4] = i >= 9 ? char((i + 1) / 10 + '0') : ' ';
    row[2 * n + 5] = i >= 9 ? char((i + 1) % 10 + '0') : char(i + 1 + '0');

    SetConsoleCursorPosition(con, cursorPos);
    WriteConsole(con, row, 2 * n + 6, &written, nullptr);
  }

  cursorPos.X += 2;
  cursorPos.Y++;
  SetConsoleCursorPosition(con, cursorPos);
  WriteConsole(con, hor_border, 2 * n + 2, &written, nullptr);

  cursorPos.X++;
  cursorPos.Y++;
  SetConsoleCursorPosition(con, cursorPos);
  WriteConsole(con, letters, 2 * n, &written, nullptr);
}

void Printer::printLegend(HANDLE con, Player *p1, Player *p2) {
  DWORD written;

  cursorPos.X = xPos == 50 ? 3 : 50;
  cursorPos.Y = yPos;

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR arrs[] = "Anton Matsviayuk S191550";
    WriteConsole(con, arrs, sizeof(arrs) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 2, con);
    TCHAR arrs[] = "Arrows to move the cursor";
    WriteConsole(con, arrs, sizeof(arrs) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR quit[] = "q: quit";
    WriteConsole(con, quit, sizeof(quit) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR n_game[] = "n: start a new game";
    WriteConsole(con, n_game, sizeof(n_game) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR confirm[] = "enter: confirm your choise";
    WriteConsole(con, confirm, sizeof(confirm) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR escape[] = "escape: cancel your choise";
    WriteConsole(con, escape, sizeof(escape) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR place[] = "i: place a stone";
    WriteConsole(con, place, sizeof(place) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR save[] = "s: save the game";
    WriteConsole(con, save, sizeof(save) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 1, con);
    TCHAR load[] = "l: load the game";
    WriteConsole(con, load, sizeof(load) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 2, con);
    TCHAR text[] = "Current cursor position:";
    WriteConsole(con, text, sizeof(text) / sizeof(TCHAR) - 1, &written, nullptr);

    TCHAR position[] = "   :   ";

    size_t y_size = (cursor_y + 1) / 10 > 0 ? 2 : 1;
    position[1] = char('A' + cursor_x);
    itoa(cursor_y + 1, position + 7 - y_size, 10);
    WriteConsole(con, position, sizeof(position) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X, cursorPos.Y + 2, con);
    TCHAR score[] = "Score:  X :  O";
    WriteConsole(con, score, sizeof(score) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(cursorPos.X + 7, cursorPos.Y + 1, con);
    TCHAR numbers[] = "    :   ";
    size_t p1_value_size = p1->getScore() / 10 > 0 ? 2 : 1;
    size_t p2_value_size = p2->getScore() / 10 > 0 ? 2 : 1;
    itoa(p1->getScore(), numbers + 2 - p1_value_size, 10);
    itoa(p2->getScore(), numbers + 8 - p2_value_size, 10);
    WriteConsole(con, numbers, sizeof(numbers) / sizeof(TCHAR) - 1, &written, nullptr);
  }
}

int Printer::requestFieldSize(HANDLE con) {
  DWORD written;
  {
    moveCursor(0, 0, con);
    TCHAR request[] = "Choose field size";
    WriteConsole(con, request, sizeof(request) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(0, 1, con);
    TCHAR request[] = "-> 9 x 9";
    WriteConsole(con, request, sizeof(request) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(0, 2, con);
    TCHAR request[] = "-> 13 x 13";
    WriteConsole(con, request, sizeof(request) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(0, 3, con);
    TCHAR request[] = "-> 19 x 19";
    WriteConsole(con, request, sizeof(request) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  {
    moveCursor(0, 4, con);
    TCHAR request[] = "-> Enter your own size (max size 26, min - 3)";
    WriteConsole(con, request, sizeof(request) / sizeof(TCHAR) - 1, &written, nullptr);
  }

  while (true) {
    if (kbhit()) {
      switch (getch()) {
        case UP: {
          moveCursor(cursorPos.X, cursorPos.Y - 1, con);
          break;
        }
        case DOWN: {
          moveCursor(cursorPos.X, cursorPos.Y + 1, con);
          break;
        }
        case LEFT: {
          moveCursor(cursorPos.X - 1, cursorPos.Y, con);
          break;
        }
        case RIGHT: {
          moveCursor(cursorPos.X + 1, cursorPos.Y, con);
          break;
        }
        case ENTER: {
          if (cursorPos.Y < 1 || cursorPos.Y > 4) {
            break;
          }
          switch (cursorPos.Y) {
            case 1: {
              system("cls");
              return 9;
            }
            case 2: {
              system("cls");
              return 13;
            }
            case 3: {
              system("cls");
              return 19;
            }
            case 4: {
              moveCursor(46, 4, con);
              char *buf = new char[3];
              gets_s(buf, 3);
              int sz = atoi(buf);
              if (sz >= 27 || sz <= 2) {
                moveCursor(46, 4, con);
                TCHAR blank[] = "   ";
                WriteConsole(con, blank, sizeof(blank) / sizeof(TCHAR) - 1, &written, nullptr);
                moveCursor(46, 4, con);
                break;
              }
              system("cls");
              return sz;
            }
          }
          break;
        }
      }
    }
  }
}

char Printer::getP1Icon() {
  return player1Icon;
}
char Printer::getP2Icon() {
  return player2Icon;
}
char Printer::getEmptyIcon() {
  return emptyCellIcon;
};
#include <windows.h>
#include <conio.h>
#include "board.cpp"
#include "player.h"
#include "printer.cpp"

using namespace std;

int main() {
  HANDLE hWindowConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD crd = {120, 40};
  SMALL_RECT src = {0, 0, static_cast<short>(crd.X - 1), static_cast<short>(crd.Y - 1)};
  SetConsoleWindowInfo(hWindowConsole, true, &src);
  SetConsoleScreenBufferSize(hWindowConsole, crd);
  DWORD written;

  Printer printer = Printer();
  int n = printer.requestFieldSize(hWindowConsole);
  Board board = Board(n);

  auto *p1 = new Player(),
      *p2 = new Player();

  printer.printLegend(hWindowConsole, p1, p2);
  printer.printBoard(hWindowConsole, board, p1);
  moveCursor(xPos, yPos + 2, hWindowConsole);

  bool turn = false;  // true for player 2, false for player 1
  bool cell_in_stack = false;
  while (true) {
    int movement = board.KeyPressed(hWindowConsole, turn ? p2 : p1);

    if (movement == EXIT_QUIT) {
      break;
    }

    switch (movement) {
      case EXIT_MOVE: {
        auto temp_x = cursorPos.X;
        auto temp_y = cursorPos.Y;
        moveCursor(cursorPos.X, cursorPos.Y, hWindowConsole);
        printer.printLegend(hWindowConsole, p1, p2);
        moveCursor(temp_x, temp_y, hWindowConsole);
        break;
      }
      case EXIT_NEW_GAME: {
        moveCursor(xPos, yPos, hWindowConsole);
        p1->getScore() = 0;
        p2->getScore() = 0;
        cursor_x = 0;
        cursor_y = 0;
        printer.printBoard(hWindowConsole, board, p1);
        printer.printLegend(hWindowConsole, p1, p2);
        moveCursor(xPos, yPos + 2, hWindowConsole);
        turn = false;
        break;
      }
      case EXIT_PLACE: {
        if (!cell_in_stack) {
          auto temp_x = cursorPos.X;
          auto temp_y = cursorPos.Y;

          moveCursor(xPos, yPos, hWindowConsole);
          printer.printBoard(hWindowConsole, board, p1);
          moveCursor(temp_x, temp_y, hWindowConsole);
        }
        cell_in_stack = true;
        break;
      }
      case EXIT_ENTER: {
        if (cell_in_stack) {
          if (board.IsKilled(turn ? p1 : p2, turn ? p2 : p1)) {
            auto temp_x = cursorPos.X;
            auto temp_y = cursorPos.Y;

            moveCursor(xPos, yPos, hWindowConsole);
            printer.printBoard(hWindowConsole, board, p1);
            printer.printLegend(hWindowConsole, p1, p2);
            moveCursor(temp_x, temp_y, hWindowConsole);
          }
          turn = !turn;
          cell_in_stack = false;
        }
        break;
      }
      case EXIT_ESC: {
        cell_in_stack = false;
        auto temp_x = cursorPos.X;
        auto temp_y = cursorPos.Y;
        moveCursor(cursorPos.X, cursorPos.Y, hWindowConsole);
        printer.printBoard(hWindowConsole, board, p1);
        moveCursor(temp_x, temp_y, hWindowConsole);
        break;
      }
      case EXIT_SAVE: {
        auto temp_x = cursorPos.X;
        auto temp_y = cursorPos.Y;

        moveCursor(0, 0, hWindowConsole);
        TCHAR score[] = "Enter file name to save the game:";
        WriteConsole(hWindowConsole, score, sizeof(score) / sizeof(TCHAR) - 1, &written, nullptr);
        char* buf = new char[5];
        gets_s(buf, 5);
        moveCursor(0, 0, hWindowConsole);

        FILE* f = fopen(buf, "a+");
        if (f != nullptr) {
          system("cls");

          moveCursor(xPos, yPos, hWindowConsole);
          board.saveBoard(f, turn, p1, p2);
          printer.printBoard(hWindowConsole, board, p1);
          printer.printLegend(hWindowConsole, p1, p2);
          moveCursor(temp_x, temp_y, hWindowConsole);

          fclose(f);
        }

        break;
      }
      case EXIT_LOAD: {
        auto temp_x = cursorPos.X;
        auto temp_y = cursorPos.Y;

        moveCursor(0, 0, hWindowConsole);
        TCHAR score[] = "Enter file name to load the game:";
        WriteConsole(hWindowConsole, score, sizeof(score) / sizeof(TCHAR) - 1, &written, nullptr);
        char* buf = new char[5];
        gets_s(buf, 5);

        FILE* f = fopen(buf, "r+");
        if (f != nullptr) {
          system("cls");

          board.loadBoard(f, turn, p1, p2);
          moveCursor(xPos, yPos, hWindowConsole);
          printer.printBoard(hWindowConsole, board, p1);
          printer.printLegend(hWindowConsole, p1, p2);
          moveCursor(temp_x, temp_y, hWindowConsole);

          fclose(f);
        }

        break;
      }
    }
  }

  delete p1;
  delete p2;

  return 0;
}
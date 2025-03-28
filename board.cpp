#include "board.h"
#include <string>

using namespace std;

bool Cell::occupy(Player *&p) {
  if (this->occupied) return false;

  this->occupied = true;
  this->playerOwned = p;

  return true;
}

void Cell::free() {
  this->occupied = false;
  this->playerOwned = NULL;
}

Player *Cell::changePossession(Player *newPlayer) {
  Player *oldPlayer = this->playerOwned;

  this->occupy(newPlayer);

  return oldPlayer; // may be NULL
}

bool Cell::isOccupied() {
  return this->occupied;
}

bool Cell::belongsTo(const Player *p) {
  return p == this->playerOwned;
}

int Cell::getRow() {
  return this->row_coordinate;
}

int Cell::getColumn() {
  return this->column_coordinate;
}

Player *Cell::getPlayerOwned() {
  return playerOwned;
}

Board::Board(int size)
    : size(size), occupiedCells(0) {

  table = new Cell *[size];

  for (int i = 0; i < size; i++) {
    table[i] = (Cell *) malloc(size * sizeof(Cell));
    for (int j = 0; j < size; j++) {
      table[i][j] = Cell(i, j);
    }
  }
}

Board::~Board() {}

int Board::getSize() { return size; }
bool Board::isFull() { return occupiedCells == size * size; }
bool Board::isEmpty() { return occupiedCells == 0; }

bool Board::isCellOwnedBy(const int row, const int col, const Player *p) {

  return getCell(row, col).belongsTo(p);
}

bool Board::isCellOccupied(const int row, const int col) {
  return getCell(row, col).isOccupied();
}

int Board::countStonesFor(const Player *p) {

  int count = 0;
  for (int row = 0; row < size; row++) {
    for (int col = 0; col < size; col++) {
      if (getCell(row, col).belongsTo(p)) {
        count++;
      }
    }
  }
  return count;
}

Player *Board::removeStone(const int row, const int col) {
  Cell &c = getCell(row, col);
  Player *p = c.getPlayerOwned();
  c.free();
  return p;
}

bool Board::playAt(const int row, const int col, Player *p) {
  Cell &c = getCell(row, col);

  if (c.isOccupied()) {
    return false;
  } else {
    c.occupy(p);
    return true;
  }
}

Cell &Board::getCell(const int row, const int col) {
  return table[row][col];
}

void Board::Clear() {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      table[i][j].free();
    }
  }
}

bool Board::InBorder(const int &x, const int &y) {
  int n = size;
  return x >= 0 && x < n && y >= 0 && y < n;
}

int Board::KeyPressed(HANDLE con, Player *&p) {
  if (kbhit()) {
    switch (getch()) {
      case UP: {
        if (InBorder(cursor_x, cursor_y - 1)) {
          cursorPos.Y--;
          cursor_y--;
        }
        return EXIT_MOVE;
      }
      case DOWN: {
        if (InBorder(cursor_x, cursor_y + 1)) {
          cursorPos.Y++;
          cursor_y++;
        }
        return EXIT_MOVE;
      }
      case LEFT: {
        if (InBorder(cursor_x - 1, cursor_y)) {
          cursorPos.X -= 2;
          cursor_x--;
        }
        return EXIT_MOVE;
      }
      case RIGHT: {
        if (InBorder(cursor_x + 1, cursor_y)) {
          cursorPos.X += 2;
          cursor_x++;
        }
        return EXIT_MOVE;
      }
      case PLACE: {
        if ((last_inserted_x != NULL && last_inserted_y != NULL) || (Ko && CheckKo(cursor_y, cursor_x, p))
            || (CalcLiberties(cursor_y, cursor_x, p) == 0 && !CheckKo(cursor_y, cursor_x, p))
            || !playAt(cursor_y, cursor_x, p)) {
          return EXIT_DEFAULT;
        }

        if (CheckKo(cursor_y, cursor_x, p)) {
          Ko = true;
        }

        last_inserted_x = cursor_x;
        last_inserted_y = cursor_y;

        return EXIT_PLACE;
      }
      case NEW_GAME: {
        Clear();
        return EXIT_NEW_GAME;
      }
      case QUIT: {
        return EXIT_QUIT;
      }
      case ENTER: {
        last_inserted_x = NULL;
        last_inserted_y = NULL;
        return EXIT_ENTER;
      }
      case ESC: {
        if (last_inserted_x != NULL && last_inserted_y != NULL) {
          table[last_inserted_y][last_inserted_x].free();
        }
        last_inserted_x = NULL;
        last_inserted_y = NULL;
        return EXIT_ESC;
      }
      case SAVE: {
        return EXIT_SAVE;
      }
      case LOAD: {
        return EXIT_LOAD;
      }
    }

    return EXIT_DEFAULT;
  }

  return EXIT_DEFAULT;
}

int Board::CalcLiberties(const int &row, const int &col, Player *p) {
  return (InBorder(row - 1, col) && ((getCell(row - 1, col).belongsTo(p)) || !getCell(row - 1, col).isOccupied())) +
      (InBorder(row + 1, col) && ((getCell(row + 1, col).belongsTo(p)) || !getCell(row + 1, col).isOccupied())) +
      (InBorder(row, col - 1) && ((getCell(row, col - 1).belongsTo(p)) || !getCell(row, col - 1).isOccupied())) +
      (InBorder(row, col + 1) && ((getCell(row, col + 1).belongsTo(p)) || !getCell(row, col + 1).isOccupied()));
}

bool Board::IsKilled(Player *p1, Player *p2) {
  if (InBorder(cursor_x, cursor_y + 1) && CalcLiberties(cursor_y + 1, cursor_x, p1) == 0) {
    removeStone(cursor_y + 1, cursor_x);
    p2->getScore()++;
    return true;
  }
  if (InBorder(cursor_x, cursor_y - 1) && CalcLiberties(cursor_y - 1, cursor_x, p1) == 0) {
    removeStone(cursor_y - 1, cursor_x);
    p2->getScore()++;
    return true;
  }
  if (InBorder(cursor_x + 1, cursor_y) && CalcLiberties(cursor_y, cursor_x + 1, p1) == 0) {
    removeStone(cursor_y, cursor_x + 1);
    p2->getScore()++;
    return true;
  }
  if (InBorder(cursor_x - 1, cursor_y) && CalcLiberties(cursor_y, cursor_x - 1, p1) == 0) {
    removeStone(cursor_y, cursor_x - 1);
    p2->getScore()++;
    return true;
  }
  return false;
}

bool Board::CheckKo(const int &row, const int &col, Player *p) {
  if (CalcLiberties(row, col, p) != 0) {
    return false;
  }

  return (
      // on the left
      (InBorder(row - 1, col - 1) && getCell(row - 1, col - 1).belongsTo(p) &&
          InBorder(row, col - 2) && getCell(row, col - 2).belongsTo(p) &&
          InBorder(row + 1, col - 1) && getCell(row + 1, col - 1).belongsTo(p)) ||
      // on the right
      (InBorder(row - 1, col + 1) && getCell(row - 1, col + 1).belongsTo(p) &&
          InBorder(row, col + 2) && getCell(row, col + 2).belongsTo(p) &&
          InBorder(row + 1, col + 1) && getCell(row + 1, col + 1).belongsTo(p)) ||
      // upper
      (InBorder(row - 1, col - 1) && getCell(row - 1, col - 1).belongsTo(p) &&
          InBorder(row - 2, col) && getCell(row - 2, col).belongsTo(p) &&
          InBorder(row + 1, col - 1) && getCell(row + 1, col - 1).belongsTo(p)) ||
      // lower
      (InBorder(row + 1, col - 1) && getCell(row + 1, col - 1).belongsTo(p) &&
          InBorder(row + 2, col) && getCell(row + 2, col).belongsTo(p) &&
          InBorder(row + 1, col + 1) && getCell(row + 1, col + 1).belongsTo(p))
          );
}

void Board::saveBoard(FILE *&f, bool turn, Player *p, Player *p2) {
  const char *flag = turn ? "1" : "0";
  const char *is_Ko = Ko ? "1" : "0";
  char sz = char('A' + size - 1);
  fwrite(flag, 1, 1, f);
  fwrite(is_Ko, 1, 1, f);
  fwrite(&sz, 1, 1, f);

  const char* new_line = "\n";
  fwrite(new_line, 1, 1, f);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      Cell &c = getCell(i, j);
      if (!c.isOccupied()) {
        fwrite(&emptyIcon, 1, 1, f);
      } else if (c.belongsTo(p)) {
        fwrite(&p1Icon, 1, 1, f);
      } else {
        fwrite(&p2Icon, 1, 1, f);
      }
    }
    fwrite(new_line, 1, 1, f);
  }

  char *nums = new char[7];
  sprintf(nums, "%d:%d\n", p->getScore(), p2->getScore());
  fwrite(nums, sizeof(nums), 1, f);
}

int Board::loadBoard(FILE *&f, bool &turn, Player *&p1, Player *&p2) {
  char *trn = new char[11 + size * (size + 1)];
  int count = fread(trn, 11 + size * (size + 1), 1, f);
  if (trn[2] - 'A' != size - 1) {
    return -1;
  }
  turn = trn[0] == '1';
  Ko = trn[1] == '1';

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      Cell c = Cell(i, j);
      switch (trn[4 + i * (size + 1) + j]) {
        case emptyIcon: {
          c.free();
          break;
        }
        case p1Icon: {
          c.occupy(p1);
          break;
        }
        case p2Icon: {
          c.occupy(p2);
          break;
        }
        default:
          break;
      }
      table[i][j] = c;
    }
  }

  p1->getScore() = 0;
  p2->getScore() = 0;
  int pow10 = 1;
  int delim_pos = 0;
  int end_of_line = 0;
  for (int i = 0; i < 7; i++) {
    if (trn[4 + size * (size + 1) + i] == ':') {
      delim_pos = size * (size + 1) + i + 4;
    }
    if (trn[4 + size * (size + 1) + i] == '\n') {
      end_of_line = size * (size + 1) + i + 3;
    }
  }
  for (int i = delim_pos - 1; trn[i] >= '0' && trn[i] <= '9'; i--) {
    p1->getScore() += (trn[i] - '0') * pow10;
    pow10 *= 10;
  }
  pow10 = 1;
  for (int i = end_of_line; trn[i] != ':'; i--) {
    p2->getScore() += (trn[i] - '0') * pow10;
    pow10 *= 10;
  }

  delete[] trn;
  return 0;
}
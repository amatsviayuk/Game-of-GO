#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <utility>

class Player {
 private:
  std::string name;
  int value;

 public:
  Player()
      : value(0) {
  }

  explicit Player(std::string name)
      : name(std::move(name)), value(0) {
  }

  ~Player() = default;

  std::string getName() const { return this->name; }
  int& getScore() { return value; }

  bool operator==(const Player &other) { return (this == &other); }
  bool operator!=(const Player &other) { return !(*this == other); }
};

#endif // PLAYER_H

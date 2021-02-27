#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <ncurses.h>

#include <iostream>
#include <unistd.h>
#include <random>
#include <vector>
#include <ctime>

namespace rng {
  inline std::mt19937_64 mersenne(static_cast<std::mt19937_64::result_type>(std::time(0)));
} // namespace rng

namespace constants {
  inline constexpr int WIDTH = 36;
  inline constexpr int HEIGHT = 16;
  inline constexpr char SNAKE_SEGMENT_CHAR = 'S';
  inline constexpr char FOOD_CHAR = '*';
} // namespace constants

struct SnakeSegment {
 int y = 0;
 int x = 0;

 SnakeSegment() = default;
 SnakeSegment(int y, int x) : y(y), x(x) {}
};

class Snake {
private:
  int score, delay;
  char direction;
  bool caughtFood;
  SnakeSegment food;
  std::vector<SnakeSegment> snake;
  WINDOW* window;

  void generateFood();
  bool detectCollision();
  void moveSnake();

public:
  Snake();
  ~Snake();

  void startGame();
};

#endif

#include "Snake.hpp"

Snake::Snake() : score(0), delay(100'000), direction('L'), caughtFood(false) {
  initscr();
  curs_set(false);
  noecho();

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  int snakeSegments = 5;

  window = newwin(constants::HEIGHT, constants::WIDTH, yMax / 2 - constants::HEIGHT / 2, xMax / 2 - constants::WIDTH / 2);

  box(window, 0, 0);
  nodelay(window, true);
  keypad(window, true);

  /* Creating snake and putting it on board. */
  for (int i = 0; i < snakeSegments; ++i) {
    snake.emplace_back(SnakeSegment(constants::HEIGHT / 2, (constants::WIDTH / 2) + i - snakeSegments / 2));
    mvwaddch(window, snake[i].y, snake[i].x, constants::SNAKE_SEGMENT_CHAR);
  }

  generateFood();
  mvwprintw(window, constants::HEIGHT - 1, constants::WIDTH / 2 - 6, "  Score: %d  ", score);
  wrefresh(window);
}

Snake::~Snake() {
  nodelay(window, false);
  delwin(window);
  endwin();
}

void Snake::generateFood() {
  bool invalidPlace = false;
  std::uniform_int_distribution randomX(1 , constants::WIDTH - 2);
  std::uniform_int_distribution randomY(1 , constants::HEIGHT - 2);

  while (true) {
    int tmpX = randomX(rng::mersenne);
    int tmpY = randomY(rng::mersenne);

    for (std::size_t i = 0; i < snake.size(); ++i) {
      if (snake[i].x == tmpX && snake[i].y == tmpY) {
        invalidPlace = true;
      }
    }

    if (invalidPlace) {
      invalidPlace = false;
      continue;
    }

    food.x = tmpX;
    food.y = tmpY;
    break;
  }

  mvwaddch(window, food.y, food.x, constants::FOOD_CHAR);
  wrefresh(window);
}

bool Snake::detectCollision() {
  /* Collision of snake with itself. */
  for (std::size_t i = 2; i < snake.size(); ++i) {
    if (snake[i].x == snake.front().x && snake[i].y == snake.front().y) {
      return true;
    }
  }

  /* Collision of snake with food. */
  if (snake.front().x == food.x && snake.front().y == food.y) {
    caughtFood = true;
    generateFood();
    score += 10;
    mvwprintw(window, constants::HEIGHT - 1, constants::WIDTH / 2 - 6, "  Score: %d  ", score);

    if ((score % 100) == 0) {
      delay -= 10'000;
    }
  } else {
    caughtFood = false;
  }

  return false;
}

void Snake::moveSnake() {
  switch(wgetch(window)) {
    case KEY_LEFT:
    if (direction != 'R') {
      direction = 'L';
    }
    break;
    case KEY_RIGHT:
    if (direction != 'L') {
      direction = 'R';
    }
    break;
    case KEY_UP:
    if (direction != 'D') {
      direction = 'U';
    }
    break;
    case KEY_DOWN:
    if (direction != 'U') {
      direction = 'D';
    }
    break;
  }

  if (!caughtFood) {
    mvwaddch(window, snake.back().y, snake.back().x, ' ');
    wrefresh(window);
    snake.pop_back();
  }

  int posX = snake.front().x;
  int posY = snake.front().y;

  if (direction == 'L') {
    posX = (snake.front().x - 1 <= 0) ? constants::WIDTH - 2 : snake.front().x - 1;
  } else if (direction == 'R') {
    posX = (snake.front().x + 1 >= constants::WIDTH - 1) ? 1 : snake.front().x + 1;
  } else if (direction == 'U') {
    posY = (snake.front().y - 1 <= 0) ? constants::HEIGHT - 2 : snake.front().y - 1;
  } else if (direction == 'D') {
    posY = (snake.front().y + 1 >= constants::HEIGHT - 1) ? 1 : snake.front().y + 1;
  }

  snake.insert(snake.begin(), SnakeSegment(posY, posX));
  mvwaddch(window, snake.front().y, snake.front().x, constants::SNAKE_SEGMENT_CHAR);
  wrefresh(window);
}

void Snake::startGame() {
  while (!detectCollision()) {
    moveSnake();
    usleep(delay);
  }

  mvwprintw(window, constants::HEIGHT / 2, constants::WIDTH / 2 - 5, "Game Over!");
  nodelay(window, false);
  wgetch(window);
}

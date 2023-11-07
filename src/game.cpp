#include "game.h"
#include "SDL.h"
#include <iostream>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)), random_sign(0, 1),
      random_axis(0, 1) {
  PlaceFood();
  obstacle_number = 0;
  obstacle_size = 0;
  obstacles = std::vector<std::vector<SDL_Point>>(
      obstacle_number, std::vector<SDL_Point>(obstacle_size));
  PlaceObstacle();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    if (snake.alive)
      renderer.Render(snake, food, obstacles);
    else {
      renderer.GameOver(score);
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceObstacle() {
  int x, y, i = 0, x_y_rand = 0, plus_minus_rand = 0;
  for (std::vector<SDL_Point> &obst_num : obstacles) {
    while (true) {
      i = 0;
      x = random_w(engine);
      y = random_h(engine);
      // Check that the location is not occupied by a snake item before
      // placing obstacle.
      for (auto &obst : obst_num) {
        if (!snake.SnakeCell(x, y)) {
          obst.x = x;
          obst.y = y;
          i++;
        } else {
          break;
        }
        /*x axis*/
        if (random_axis(engine)) {
          /* + sign */
          if (random_sign(engine))
            x++;
          else
            x--;
        }
        /* y axis */
        else {
          /* + sign*/
          if (random_sign(engine))
            y++;
          else
            y--;
        }
      }
      if (i == obstacle_size)
        break;
    }
  }
}

void Game::Update() {
  if (!snake.alive)
    return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if the snake has died due to hitting obstcales.

  for (auto const &obst_num : obstacles) {
    for (auto const &obst : obst_num) {
      if (new_x == obst.x && new_y == obst.y) {
        snake.alive = false;
        break;
      }
    }
    if (snake.alive == false)
      break;
  }

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;

    switch (score) {
    case 2:
    case 10:
    case 20:
    case 25:
    case 30:
      IncrementObstsNum();
      break;

    case 4:
    case 7:
    case 14:
    case 17:
    case 23:
      IncrementObstSize();
      break;

    default:
      break;
    }

    /* Place food and obstacle*/
    PlaceFood();
    PlaceObstacle();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::IncrementObstSize() {
  obstacle_size++;
  for (auto &obst : obstacles) {
    obst.resize(obstacle_size);
  }
}
void Game::IncrementObstsNum() {
  obstacle_number++;
  obstacles.resize(obstacle_number);
  if (obstacle_size == 0) {
    IncrementObstSize();
  } else {
    for (auto &obst : obstacles) {
      obst.resize(obstacle_size);
    }
  }
}
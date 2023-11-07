#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "pacman.h"
#include <random>

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

  void IncrementObstsNum();
  void IncrementObstSize();

private:
  Pacman pacman;
  SDL_Point food;
  std::vector<std::vector<SDL_Point>> obstacles;
  int obstacle_size;   /*number of dots per one obstacle*/
  int obstacle_number; /* total numebr of obstacles*/

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::uniform_int_distribution<int> random_sign;
  std::uniform_int_distribution<int> random_axis;

  int score{0};

  void PlaceFood();
  void PlaceObstacle();
  void Update();
};

#endif
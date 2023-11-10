#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "pacman.h"

void Controller::ChangeDirection(Pacman &pacman, Pacman::Direction input,
                                 Pacman::Direction opposite) const {
  //if (pacman.direction != opposite || pacman.size == 1) 
  pacman.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Pacman &pacman) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
              pacman.direction=Pacman::Direction::kUp;
              pacman.angel=270;
              pacman.flip=SDL_FLIP_VERTICAL;
          break;

        case SDLK_DOWN:
              pacman.direction=Pacman::Direction::kDown;
              pacman.angel=90;
              pacman.flip=SDL_FLIP_VERTICAL;

          break;

        case SDLK_LEFT:
            pacman.direction=Pacman::Direction::kLeft;
              pacman.angel=0;
              pacman.flip=SDL_FLIP_HORIZONTAL;

          break;

        case SDLK_RIGHT:
          pacman.direction=Pacman::Direction::kRight;
              pacman.angel=0;
              pacman.flip=SDL_FLIP_NONE;

          break;
      }
    }
  }
}
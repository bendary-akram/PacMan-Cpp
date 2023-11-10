#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "pacman.h"
#include "string"
#include <vector>

class Renderer {
public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Pacman const pacman, SDL_Point const &food,
              std::vector<std::vector<SDL_Point>> const &obstacles);
  void UpdateWindowTitle(int score, int fps);
  void GameOver(int score);

  bool loadImage(std::string filename);

private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  TTF_Font *font;
  SDL_Surface *surfaceMessage;
  SDL_Surface * image;
  SDL_Texture *Message;
};

#endif
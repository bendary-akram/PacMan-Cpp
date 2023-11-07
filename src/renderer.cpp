#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width), screen_height(screen_height),
      grid_width(grid_width), grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  if (TTF_Init() < 0) {
    std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
  };
  font = TTF_OpenFont("../src/Arial.ttf", 24);
  if (!font) {
    std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
  }
  // Create Window
  sdl_window = SDL_CreateWindow("Pacman Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}

void Renderer::Render(Pacman const pacman, SDL_Point const &food,
                      std::vector<std::vector<SDL_Point>> const &obstacles) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render obstacle
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0x00);
  for (auto obst_num : obstacles) {
    for (auto obst : obst_num) {
      block.x = obst.x * block.w;
      block.y = obst.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
    }
  }
  // Render pacman's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : pacman.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render pacman's head
  block.x = static_cast<int>(pacman.head_x) * block.w;
  block.y = static_cast<int>(pacman.head_y) * block.h;
  if (pacman.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Pacman Score: " + std::to_string(score) +
                    " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
void Renderer::GameOver(int score) {
  SDL_Color White = {255, 255, 255};

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  surfaceMessage = TTF_RenderText_Solid(font, "Game Over", White);
  if (!surfaceMessage) {
    std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
  }

  Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage);

  SDL_Rect Message_rect;              // create a rect
  Message_rect.x = 250;               // controls the rect's x coordinate
  Message_rect.y = 250;               // controls the rect's y coordinte
  Message_rect.w = surfaceMessage->w; // controls the width of the rect
  Message_rect.h = surfaceMessage->h; // controls the height of the rect

  SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);

  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);

  std::string str = " Score: " + std::to_string(score);
  surfaceMessage = TTF_RenderText_Solid(font, str.c_str(), White);
  if (!surfaceMessage) {
    std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
  }
  Message = SDL_CreateTextureFromSurface(sdl_renderer, surfaceMessage);
  Message_rect.x = 250;               // controls the rect's x coordinate
  Message_rect.y = 280;               // controls the rect's y coordinte
  Message_rect.w = surfaceMessage->w; // controls the width of the rect
  Message_rect.h = surfaceMessage->h; // controls the height of the rect

  SDL_RenderCopy(sdl_renderer, Message, NULL, &Message_rect);

  SDL_RenderPresent(sdl_renderer);

  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
}
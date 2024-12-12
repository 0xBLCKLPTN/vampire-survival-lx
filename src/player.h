#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Player {
  SDL_Renderer* renderer;
  bool is_moving;
  int x, y;
  int width, height;
  SDL_Texture* stay_texture;
  int current_frame;
  float frame_delay;
  uint32_t last_frame_time;
  int direction;
  SDL_Rect* rect;
  int w, h;

  void (*move)(struct Player*, int dx, int dy);

} Player;


Player* init_player(SDL_Renderer* renderer) {
  Player* player = (Player*)malloc(sizeof(Player));

  player->renderer = renderer;
  player->current_frame = 0;
  player->frame_delay = 0.1f;
  player->last_frame_time = 0;
  // -1 - DOWN | 1 - UP
  // -1 - LEFT | 1 - RIGHT
  player->direction = -1;

  player->x = 800 / 2;
  player->y = 600 / 2;
  player->is_moving = false;

  player->stay_texture = load_image(player->renderer, "../Assets/d1.png", 64, 64);
  player->rect->x = player->x;
  player->rect->y = player->y;
  player->w = 33; player->h = 32;

  return player;
}

void move(Player* player, int dx, int dy) {
  player->x += dx;
  player->y += dy;

  if (dx > 0) player->direction = 1; // RIGHT
  if (dx < 0) player->direction = -1; // LEFT
  if (dy > 0) player->direction = -1; // DOWN
  if (dy < 0) player->direction = 1; // UP

  player->is_moving = true;
}

/*
void draw(Player* player)
{
  SDL_Rect dest_rect = {player->x - camera->x, player->y - camera->y, player->width * 2, player->height * 2};
  SDL_RendererFlip flip = SDL_FLIP_NONE;

  SDL_Texture* texture = NULL;

  SDL_RenderCopyEx(player->renderer, texture, NULL, dest_rect, 0, NULL, flip);
}
*/

void destroy_player(Player* player) {
  free(player);
}



#endif // PLAYER_H

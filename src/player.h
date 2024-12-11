#ifndef PLAYER_H_
#define PLAYER_H_


#include "./includes.h"
typedef struct {
  SDL_Renderer* renderer;
  bool is_moving;
  int x, y;
  int width, height;
  SDL_Texture* stay_texture;
  int current_frame;
  float frame_delay;
  uint32_t last_frame_time;
  int direction;
} Player;

Player* init_player(SDL_Renderer* renderer);
void move(Player* player, int dx, int dy);
void destroy_player(Player* player);

#endif // PLAYER_H_

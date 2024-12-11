#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "./includes.h"
#include "./texture.h"
#include "./player.h"


typedef struct Application{
  uint32_t width, height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool running;
  void (*run)(struct Application*);
  void (*render)(struct Application*);
  void (*handleEvent)(struct Application*);
  
} Application;

void handleEvent(Application *application);
void render(Application *application);
void run(Application *run);
Application* init_application(uint32_t width, uint32_t height, char *window_title);
void destroy_application(Application *application);




#endif // APPLICATION_H_

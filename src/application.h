#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Application {
  uint32_t width, height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool running;
  void (*run)(struct Application*);
  void (*render)(struct Application*);
  void (*handleEvent)(struct Application*);
} Application;

void handleEvent(Application* application) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) { application->running = false; }
    else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
      application->width = e.window.data1;
      application->height = e.window.data2;
      SDL_RenderSetLogicalSize(application->renderer, application->width, application->height);
    }
  }
}


void render(Application* application) {
  SDL_SetRenderDrawColor(application->renderer, 255, 0, 0, 255);
  SDL_RenderClear(application->renderer);
  SDL_RenderPresent(application->renderer);
}


void run(Application* application) {
  while (application->running) {
    application->handleEvent(application);
    application->render(application);
  }
}


Application* init_application(uint32_t width, uint32_t height, char* window_title) {
  Application* application = (Application*)malloc(sizeof(Application));

  application->width = width;
  application->height = height;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Error while loading SDL\n");
    return NULL; 
  }

  application->window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!application->window) {
    fprintf(stderr, "Error while creating window.\n");
    return NULL;
  }

  application->renderer = SDL_CreateRenderer(application->window, -1, SDL_RENDERER_ACCELERATED);
  if (!application->renderer) {
    fprintf(stderr, "Error while creating renderer\n");
    return NULL;
  }

  int imgFlags = IMG_INIT_PNG;
  SDL_Init(SDL_INIT_VIDEO);

  if (!(IMG_Init(imgFlags) & imgFlags)) {
    fprintf(stderr, "SDL image could not initialize\n");
    return NULL;
  }

  application->running = true;
  application->render = render;
  application->run = run;
  application->handleEvent = handleEvent;
  return application;
}


void destroy_application(Application* application) {
  free(application);
}



#endif // APPLICATION_H

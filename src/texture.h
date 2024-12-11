#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "./includes.h"


SDL_Texture* load_image(SDL_Renderer* renderer, char* file_path, int outWidth, int outHeight) {
  SDL_Surface* loaded_surface = IMG_Load(file_path);
  if (loaded_surface == NULL) {
    fprintf(stderr, "Unable to load image! SDL_image error: %s\n", IMG_GetError());
    return NULL;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
  SDL_FreeSurface(loaded_surface);

  if (texture == NULL) {
    fprintf(stderr, "Unable to create texture! SDL_Error: %s\n", SDL_GetError());
    return NULL;
  }

  outWidth = loaded_surface->w;
  outHeight = loaded_surface->h;

  printf("Loaded image: %s with dimensions %d x %d\n", file_path, outWidth, outHeight);
  return texture;
}

SDL_Surface* scale_surface(SDL_Surface* surface, int width, int height) {
  SDL_Surface* scaled_surface = SDL_CreateRGBSurface(0, width, height, 32,
                                                     0x00FF0000,
                                                     0x0000FF00,
                                                     0x000000FF,
                                                     0xFF000000);
  if (!scaled_surface) {
    fprintf(stderr, "Could not create scaled surface: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_SoftStretch(surface, NULL, scaled_surface, NULL);
  return scaled_surface;
}

#endif // TEXTURE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "application.h"
#include "textures.h"
#include "player.h"



int main(int argc, char* argv[]) {
	Application* application = init_application(800, 600, "Vampire-Survival-LX");
  application->run(application);
  destroy_application(application);
}

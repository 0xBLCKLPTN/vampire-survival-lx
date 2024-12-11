#include "./application.h"
#include "./texture.h"
#include "./player.h"
#include <stdint.h>




int main(int argc, char* argv[]) {
	Application* application = init_application(800, 600, "Vampire-Survival-LX");
  application->run(application);
  destroy_application(application);
}

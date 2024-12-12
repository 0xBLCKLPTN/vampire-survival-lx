#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

// Функция для загрузки изображения и создания текстуры
// Function to load an image and create a texture
SDL_Texture* load_image(SDL_Renderer* renderer, char* file_path, int* outWidth, int* outHeight) {
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

  *outWidth = loaded_surface->w;
  *outHeight = loaded_surface->h;

  printf("Loaded image: %s with dimensions %d x %d\n", file_path, *outWidth, *outHeight);
  return texture;
}

// Функция для масштабирования поверхности
// Function to scale a surface
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

// Структура для представления игрока
// Structure to represent a player
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
  SDL_Rect rect;
  int w, h;

  void (*move)(struct Player*, int dx, int dy);

} Player;

// Структура для представления объекта
// Structure to represent an object
typedef struct Object {
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  SDL_Rect rect;
} Object;

// Функция для перемещения игрока
// Function to move the player
void move(Player* player, int dx, int dy) {
  player->x += dx;
  player->y += dy;

  if (dx > 0) player->direction = 1; // ВПРАВО
  if (dx < 0) player->direction = -1; // ВЛЕВО
  if (dy > 0) player->direction = -1; // ВНИЗ
  if (dy < 0) player->direction = 1; // ВВЕРХ

  player->is_moving = true;

  // Обновление позиции прямоугольника игрока
  // Update the player's rectangle position
  player->rect.x = player->x;
  player->rect.y = player->y;
}

// Функция для инициализации игрока
// Function to initialize the player
Player* init_player(SDL_Renderer* renderer) {
  Player* player = (Player*)malloc(sizeof(Player));

  player->renderer = renderer;
  player->current_frame = 0;
  player->frame_delay = 0.1f;
  player->last_frame_time = 0;
  // -1 - ВНИЗ | 1 - ВВЕРХ
  // -1 - ВЛЕВО | 1 - ВПРАВО
  player->direction = -1;

  player->x = 800 / 2;
  player->y = 600 / 2;
  player->is_moving = false;

  int width, height;
  player->stay_texture = load_image(player->renderer, "../Assets/d1.png", &width, &height);
  player->rect.x = player->x;
  player->rect.y = player->y;
  player->rect.w = width;
  player->rect.h = height;
  player->w = width;
  player->h = height;

  player->move = move;

  return player;
}

// Функция для освобождения ресурсов игрока
// Function to free player resources
void destroy_player(Player* player) {
  SDL_DestroyTexture(player->stay_texture);
  free(player);
}

// Функция для создания объекта
// Function to create an object
Object* create_object(SDL_Renderer* renderer, char* file_path, int x, int y) {
  Object* object = (Object*)malloc(sizeof(Object));
  object->renderer = renderer;
  int width, height;
  object->texture = load_image(renderer, file_path, &width, &height);
  object->rect.x = x;
  object->rect.y = y;
  object->rect.w = width;
  object->rect.h = height;
  return object;
}

// Функция для освобождения ресурсов объекта
// Function to free object resources
void destroy_object(Object* object) {
  SDL_DestroyTexture(object->texture);
  free(object);
}

// Структура для представления приложения
// Structure to represent the application
typedef struct Application {
  uint32_t width, height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool running;
  void (*run)(struct Application*);
  void (*render)(struct Application*);
  void (*handleEvent)(struct Application*);
  Player* player;
  Object** objects;
  int object_count;
  const Uint8* key_states; // Добавьте эту строку
} Application;

// Функция для обработки событий
// Function to handle events
void handleEvent(Application* application) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      application->running = false;
    } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
      application->width = e.window.data1;
      application->height = e.window.data2;
      SDL_RenderSetLogicalSize(application->renderer, application->width, application->height);
    }
  }

  // Обновление состояния клавиш
  // Update key states
  application->key_states = SDL_GetKeyboardState(NULL);
}

// Функция для рисования шахматной доски
// Function to draw a chessboard
void draw_chessboard(SDL_Renderer* renderer, int width, int height, int square_size) {
  SDL_Rect rect;
  rect.w = square_size;
  rect.h = square_size;

  for (int y = 0; y < height; y += square_size) {
    for (int x = 0; x < width; x += square_size) {
      rect.x = x;
      rect.y = y;
      if ((x / square_size + y / square_size) % 2 == 0) {
        SDL_SetRenderDrawColor(renderer, 157, 230, 78, 255); // Белый Зеленый
      } else {
        SDL_SetRenderDrawColor(renderer, 38, 133, 76, 255); // Черный Зеленый
      }
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}

// Функция для рендеринга
// Function to render
void render(Application* application) {
  int square_size = 120; // Размер каждой клетки шахматной доски
  draw_chessboard(application->renderer, application->width, application->height, square_size);

  // Рендеринг игрока
  // Render player
  SDL_RenderCopyEx(application->renderer, application->player->stay_texture, NULL, &application->player->rect, 0, NULL, SDL_FLIP_NONE);

  // Рендеринг объектов
  // Render objects
  for (int i = 0; i < application->object_count; i++) {
    SDL_RenderCopyEx(application->renderer, application->objects[i]->texture, NULL, &application->objects[i]->rect, 0, NULL, SDL_FLIP_NONE);
  }

  SDL_RenderPresent(application->renderer);
}

// Функция для запуска приложения
// Function to run the application
void run(Application* application) {
  while (application->running) {
    application->handleEvent(application);

    // Обновление позиции игрока на основе состояния клавиш
    // Update player position based on key states
    if (application->key_states[SDL_SCANCODE_UP]) {
      application->player->move(application->player, 0, -1);
    }
    if (application->key_states[SDL_SCANCODE_DOWN]) {
      application->player->move(application->player, 0, 1);
    }
    if (application->key_states[SDL_SCANCODE_LEFT]) {
      application->player->move(application->player, -1, 0);
    }
    if (application->key_states[SDL_SCANCODE_RIGHT]) {
      application->player->move(application->player, 1, 0);
    }

    application->render(application);

    // Добавление небольшой задержки для контроля частоты кадров
    // Add a small delay to control the frame rate
    SDL_Delay(16); // Примерно 60 кадров в секунду
  }
}

// Функция для инициализации приложения
// Function to initialize the application
Application* init_application(uint32_t width, uint32_t height, char* window_title) {
  Application* application = (Application*)malloc(sizeof(Application));

  application->width = width;
  application->height = height;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Ошибка при загрузке SDL\n");
    return NULL;
  }

  application->window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!application->window) {
    fprintf(stderr, "Ошибка при создании окна.\n");
    return NULL;
  }

  application->renderer = SDL_CreateRenderer(application->window, -1, SDL_RENDERER_ACCELERATED);
  if (!application->renderer) {
    fprintf(stderr, "Ошибка при создании рендерера\n");
    return NULL;
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    fprintf(stderr, "SDL image не смог инициализироваться\n");
    return NULL;
  }

  application->running = true;
  application->render = render;
  application->run = run;
  application->handleEvent = handleEvent;

  application->player = init_player(application->renderer);

  // Инициализация случайного числа
  // Initialize random seed
  srand(time(NULL));

  // Создание случайного количества объектов
  // Create random number of objects
  application->object_count = rand() % 10 + 1; // Случайное количество между 1 и 10
  application->objects = (Object**)malloc(application->object_count * sizeof(Object*));

  for (int i = 0; i < application->object_count; i++) {
    int x = rand() % width;
    int y = rand() % height;
    application->objects[i] = create_object(application->renderer, "../Assets/forest.png", x, y);
  }

  return application;
}

// Функция для освобождения ресурсов приложения
// Function to free application resources
void destroy_application(Application* application) {
  destroy_player(application->player);

  for (int i = 0; i < application->object_count; i++) {
    destroy_object(application->objects[i]);
  }
  free(application->objects);

  SDL_DestroyRenderer(application->renderer);
  SDL_DestroyWindow(application->window);
  IMG_Quit();
  SDL_Quit();
  free(application);
}

// Основная функция
// Main function
int main(int argc, char* argv[]) {
  Application* application = init_application(800, 600, "Vampire-Survival-LX");
  application->run(application);
  destroy_application(application);
  return 0;
}

/*
Projeto 1
Computação Visual - Ciência da Computação
Grupo:
Matheus Santiago de Brito - 10408953
Fernando Pegoraro Bilia - 10402097
Carlos Eduardo Rosendo Basseto - 10409941
Joao Pedro Gianfaldoni - 10409524
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>

// Constants
static const char *WINDOW_TITLE = "Tela Principal";
static const char *WINDOW_TITLE2 = "Tela Secundária";
static const char *BUTTON_TEXT_EQUALIZE = "Equalizar";
static const char *BUTTON_TEXT_ORIGINAL = "Ver original";
char *IMAGE_FILENAME;
static const char *DEFAULT_OUTPUT_FILENAME = "output_image.png";

enum constants {
  DEFAULT_WINDOW_WIDTH = 0,
  DEFAULT_WINDOW_HEIGHT = 0,
  DEFAULT_WINDOW_CHILD_WIDTH = 320,
  DEFAULT_WINDOW_CHILD_HEIGHT = 240,
};

typedef struct MyWindow MyWindow;
struct MyWindow {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

typedef struct MyImage MyImage;
struct MyImage {
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_FRect rect;
};

typedef struct Button Button;
struct Button {
    SDL_FRect rect;
    SDL_Color color_normal;
    SDL_Color color_hover;
    SDL_Color color_pressed;
    
    const char *text;
    SDL_Texture *text_texture;
    int text_w;
    int text_h;
    
    bool is_hovered;
    bool is_pressed;
    bool was_clicked;
};

typedef struct Histogram Histogram;
struct Histogram {
    SDL_FRect rect;
};

// Global variables
float counterIntensity[256];
float counterIntensityEqualized[256];
bool equalized = false;
SDL_FRect histBars[256];
SDL_Surface *originalSurface;
SDL_Surface *equalizedSurface;
char *contrast;
char *brightness;

static MyWindow g_window = { .window = NULL, .renderer = NULL };
static MyWindow g_windowChild = {.window = NULL, .renderer = NULL};
static MyImage g_image = {
  .surface = NULL,
  .texture = NULL,
  .rect = { .x = 0.0f, .y = 0.0f, .w = 0.0f, .h = 0.0f }
};
static Button g_button = {
    .rect = {0, 0, 0, 0},
    .color_normal = {0, 0, 0, 0},
    .color_hover  = {0, 0, 0, 0},
    .color_pressed = {0, 0, 0, 0},
    .text = NULL,
    .text_texture = NULL,
    .text_w = 0,
    .text_h = 0,
    .is_hovered = false,
    .is_pressed = false,
    .was_clicked = false
};
static Histogram g_hist = { .rect = {0,0,0,0} };

// Function declarations
static bool MyWindow_initialize(MyWindow *window, const char *title, int width, int height, SDL_WindowFlags window_flags);
static void MyWindow_destroy(MyWindow *window);
static void MyImage_destroy(MyImage *image);
static void createButton();
static void renderButton();
static void toggleButtonText();
static void loadHistogramButton();
static SDL_AppResult initialize();
static void loadImage(const char *filename, SDL_Renderer *renderer, MyImage *output_image);
static void render();
static void createHistogram();
static void renderHistogramBars();
static void countIntensity(SDL_Surface *surface);
static void equalize(SDL_Surface *surface);
static void createTextureSurface(SDL_Renderer *renderer);
static void analyzeImage(SDL_Surface *surface);
static void renderImageStats();

// Function implementations

bool MyWindow_initialize(MyWindow *window, const char *title, int width, int height, SDL_WindowFlags window_flags) {
  SDL_Log("\tMyWindow_initialize(%s, %d, %d)", title, width, height);
  return SDL_CreateWindowAndRenderer(title, width, height, window_flags, &window->window, &window->renderer);
}

void MyWindow_destroy(MyWindow *window) {
  SDL_Log(">>> MyWindow_destroy()");
  SDL_Log("\tDestruindo MyWindow->renderer...");
  SDL_DestroyRenderer(window->renderer);
  window->renderer = NULL;
  SDL_Log("\tDestruindo MyWindow->window...");
  SDL_DestroyWindow(window->window);
  window->window = NULL;
  SDL_Log("<<< MyWindow_destroy()");
}

void MyImage_destroy(MyImage *image) {
  SDL_Log(">>> MyImage_destroy()");
  if (!image) return;
  if (image->texture) {
    SDL_DestroyTexture(image->texture);
    image->texture = NULL;
  }
  if (image->surface) {
    SDL_DestroySurface(image->surface);
    image->surface = NULL;
  }
  image->rect.x = image->rect.y = image->rect.w = image->rect.h = 0.0f;
  SDL_Log("<<< MyImage_destroy()");
}

static SDL_AppResult initialize(void) {
  SDL_Log(">>> initialize()");
  for(int i=0;i<256;i++){
    counterIntensity[i]=0;
    counterIntensityEqualized[i]=0;
  }
  if (!SDL_Init(SDL_INIT_VIDEO)) return SDL_APP_FAILURE;
  if (!MyWindow_initialize(&g_window, WINDOW_TITLE, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0)) return SDL_APP_FAILURE;
  if (!MyWindow_initialize(&g_windowChild, WINDOW_TITLE2, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 0)) return SDL_APP_FAILURE;
  if(!SDL_SetWindowParent(g_windowChild.window, g_window.window)) return SDL_APP_FAILURE;
  if(!TTF_Init()) return SDL_APP_FAILURE;
  
  SDL_Log("<<< initialize()");
  return SDL_APP_CONTINUE;
}

static void shutdown(void) {
  SDL_Log(">>> shutdown()");
  MyImage_destroy(&g_image);
  MyWindow_destroy(&g_window);
  MyWindow_destroy(&g_windowChild);
  SDL_Quit();
  SDL_Log("<<< shutdown()");
}

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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// ================= STRUCTS =================

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} MyWindow;

typedef struct {
    SDL_Texture* texture;
    int width;
    int height;
    Uint8* pixels;
} MyImage;

typedef struct {
    SDL_Rect rect;
    const char* text;
    bool isHovered;
} Button;

// ================= VARIÁVEIS GLOBAIS =================

MyWindow mainWindow;
bool isEqualized = false;

// ================= FUNÇÕES DE JANELA =================

int initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erro ao inicializar SDL\n");
        return 0;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Erro ao inicializar SDL_image\n");
        return 0;
    }

    if (TTF_Init() == -1) {
        printf("Erro ao inicializar SDL_ttf\n");
        return 0;
    }

    return 1;
}

void shutdown() {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int createWindow(MyWindow* win, const char* title, int w, int h) {
    win->window = SDL_CreateWindow(title, w, h, 0);
    if (!win->window) {
        printf("Erro ao criar janela\n");
        return 0;
    }

    win->renderer = SDL_CreateRenderer(win->window, NULL);
    if (!win->renderer) {
        printf("Erro ao criar renderer\n");
        return 0;
    }

    return 1;
}

void MyWindow_destroy(MyWindow* win) {
    SDL_DestroyRenderer(win->renderer);
    SDL_DestroyWindow(win->window);
}

// ================= ANÁLISE DE IMAGEM =================

void analyzeImage(Uint8* pixels, int totalPixels, float* media, float* desvio) {
    long soma = 0;
    long somaQuadrados = 0;

    for (int i = 0; i < totalPixels; i++) {
        soma += pixels[i];
        somaQuadrados += pixels[i] * pixels[i];
    }

    *media = (float)soma / totalPixels;

    float variancia = ((float)somaQuadrados / totalPixels) - (*media * *media);
    *desvio = sqrt(variancia);
}

void renderImageStats(float media, float desvio) {
    printf("Brilho (media): %.2f\n", media);
    printf("Contraste (desvio): %.2f\n", desvio);

    if (media < 85)
        printf("Imagem Escura\n");
    else if (media < 170)
        printf("Imagem Média\n");
    else
        printf("Imagem Clara\n");

    if (desvio < 50)
        printf("Contraste Baixo\n");
    else if (desvio < 100)
        printf("Contraste Médio\n");
    else
        printf("Contraste Alto\n");
}

// ================= BOTÃO =================

void toggleButtonText(Button* btn) {
    if (isEqualized) {
        btn->text = "Ver Original";
    }
    else {
        btn->text = "Equalizar";
    }
}

// ================= MAIN =================

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: ./main <imagem>\n");
        return 1;
    }

    if (!initialize()) {
        return 1;
    }

    if (!createWindow(&mainWindow, "Visualizador", WINDOW_WIDTH, WINDOW_HEIGHT)) {
        return 1;
    }

    bool running = true;
    SDL_Event event;

    Button btn;
    btn.rect = (SDL_Rect){ 50, 50, 200, 50 };
    btn.text = "Equalizar";

    // Simulação (substituir pelos pixels reais depois)
    Uint8 fakePixels[1000];
    for (int i = 0; i < 1000; i++) {
        fakePixels[i] = rand() % 256;
    }

    float media, desvio;
    analyzeImage(fakePixels, 1000, &media, &desvio);
    renderImageStats(media, desvio);

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= btn.rect.x && x <= btn.rect.x + btn.rect.w &&
                    y >= btn.rect.y && y <= btn.rect.y + btn.rect.h) {

                    isEqualized = !isEqualized;
                    toggleButtonText(&btn);
                }
            }
        }

        SDL_SetRenderDrawColor(mainWindow.renderer, 255, 255, 255, 255);
        SDL_RenderClear(mainWindow.renderer);

        // Desenha botão
        SDL_SetRenderDrawColor(mainWindow.renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(mainWindow.renderer, &btn.rect);

        SDL_RenderPresent(mainWindow.renderer);
    }

    MyWindow_destroy(&mainWindow);
    shutdown();

    return 0;
}


static void render(void) {
  SDL_SetRenderDrawColor(g_window.renderer, 128, 128, 128, 255);
  SDL_RenderClear(g_window.renderer);
  SDL_RenderTexture(g_window.renderer, g_image.texture, &g_image.rect, &g_image.rect);
  SDL_RenderPresent(g_window.renderer);

  SDL_SetRenderDrawColor(g_windowChild.renderer, 128, 128, 128, 255);
  SDL_RenderClear(g_windowChild.renderer);
  renderButton();
  renderHistogramBars();
  renderImageStats();
  SDL_RenderPresent(g_windowChild.renderer);
}

static void loop(void) {
  SDL_Log(">>> loop()");
  SDL_Cursor *cursor_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
  SDL_Cursor *cursor_hand  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
  bool mustRefresh = false;
  render();
  SDL_Event event;
  bool isRunning = true;

  while(isRunning) {
    float mouse_x_global, mouse_y_global;
    SDL_GetGlobalMouseState(&mouse_x_global, &mouse_y_global);
    int child_x, child_y;
    SDL_GetWindowPosition(g_windowChild.window, &child_x, &child_y);
    int mouse_x = mouse_x_global - child_x;
    int mouse_y = mouse_y_global - child_y;
    bool hovering = (mouse_x>=g_button.rect.x && mouse_x<=g_button.rect.x+g_button.rect.w &&
                     mouse_y>=g_button.rect.y && mouse_y<=g_button.rect.y+g_button.rect.h);
    if(hovering!=g_button.is_hovered) {
      g_button.is_hovered = hovering;
      SDL_SetCursor(hovering?cursor_hand:cursor_arrow);
      mustRefresh = true;
    }

 if(g_button.was_clicked) {
      toggleButtonText();
      equalized = !equalized;
      if(equalized){
        equalize(g_image.surface);
        countIntensity(equalizedSurface);
      } else {
        g_image.surface = SDL_ConvertSurface(originalSurface, SDL_PIXELFORMAT_RGBA32);
        countIntensity(originalSurface);
      }
      createTextureSurface(g_window.renderer);
      g_button.was_clicked = false;
      mustRefresh = true;
    }
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
          if(event.window.windowID == SDL_GetWindowID(g_window.window) || event.window.windowID == SDL_GetWindowID(g_windowChild.window))
            isRunning = false;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          if(g_button.is_hovered) {
            g_button.is_pressed = true; mustRefresh = true;
          }
          break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
          if(g_button.is_pressed && g_button.is_hovered) {
            g_button.was_clicked = true; g_button.is_pressed = false; mustRefresh = true;
          }
          break;
        case SDL_EVENT_KEY_DOWN:
          if(event.key.key == SDLK_S){
            SDL_ClearError();
            if (!IMG_SavePNG(g_image.surface, DEFAULT_OUTPUT_FILENAME)) {
              SDL_Log("Erro ao salvar a imagem: %s", SDL_GetError());
            } else {
              SDL_Log("Imagem salva como %s", DEFAULT_OUTPUT_FILENAME);
            }
          }
          break;
      }
    }
    if(mustRefresh) { render(); mustRefresh = false; }
    SDL_Delay(10);
  }
  SDL_DestroyCursor(cursor_arrow);
  SDL_DestroyCursor(cursor_hand);
  SDL_Log("<<< loop()");
}

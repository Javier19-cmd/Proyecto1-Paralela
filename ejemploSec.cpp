/**
 * @file ejemploSec.cpp
 * @author Javier Valle, Mario de Leon
 * @ID 20159, 19019.
 * @brief 
 * @version 0.1
 * @date 2023-08-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <cmath>

// Dimensiones de la ventana.
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

// Maximo de cuadros por segundo.
const int MIN_FPS = 60;

// Clase que representa los elementos.
class Element {
public:
    Element(int x, int y, int radius, int speedX, int speedY)
        : x(x), y(y), radius(radius), speedX(speedX), speedY(speedY) {
        // Color aleatorio para cada circulo a generar.
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
    }

    // Actualizador de la pos del elemento.
    void update() {
        x += speedX;
        y += speedY;

        if (x - radius <= 0 || x + radius >= SCREEN_WIDTH) {
            speedX = -speedX; // Cambio de dirección en X al colisionar con las paredes horizontales
        }

        if (y - radius <= 0 || y + radius >= SCREEN_HEIGHT) {
            speedY = -speedY; // Cambio de dirección en Y al colisionar con las paredes verticales
        }

        // Limitando la velocidad en las direcciones X e Y
        speedX = std::min(std::max(speedX, -5), 5);
        speedY = std::min(std::max(speedY, -5), 5);
    }

    // Renderizando el elemento en la ventana.
    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        
        // Dibujando el circulo.
        for (int i = x - radius; i <= x + radius; ++i) {
            for (int j = y - radius; j <= y + radius; ++j) {
                int dx = i - x;
                int dy = j - y;
                if (dx * dx + dy * dy <= radius * radius) {
                    SDL_RenderDrawPoint(renderer, i, j);
                }
            }
        }
    }

private:
    int x, y, radius, speedX, speedY;
    SDL_Color color;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Creando la ventana del screensaver.
    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Creando el renderizador.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Obteniendo la cantidad de elementos que ingreso el usuario. Si no ingreso nada, se ponen 50.
    int numElements = (argc > 1) ? atoi(argv[1]) : 50;
    srand(time(nullptr));

    // Creando un arreglos que tiene punteros a elementos.
    Element* elements[numElements];
    for (int i = 0; i < numElements; ++i) {

        // Generando propiedades aleatorias para cada elemento.
        int x = rand() % (SCREEN_WIDTH - 30) + 30;
        int y = rand() % (SCREEN_HEIGHT - 30) + 30;
        int radius = rand() % 10 + 5;
        int speedX = rand() % 5 + 1;
        int speedY = rand() % 5 + 1;
        elements[i] = new Element(x, y, radius, speedX, speedY);
    }

    bool quit = false;
    SDL_Event e;
    Uint32 startTime, frameTime, prevTime = 0;
    int frames = 0;

    while (!quit) {
        startTime = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limpieando el renderizador.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Actualizando y renderizando los elementos.
        for (int i = 0; i < numElements; ++i) {
            elements[i]->update();
            elements[i]->render(renderer);
        }

        // Sacando el elemento renderizado en la ventana.
        SDL_RenderPresent(renderer);

        // Calculando e imprimiendo los FPS de la pantalla.
        frames++;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - prevTime >= 1000) {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
            prevTime = currentTime;
        }

        // Limitando la velocidad de actualizacion.
        frameTime = SDL_GetTicks() - startTime;
        if (frameTime < 1000 / MIN_FPS) {
            SDL_Delay(1000 / MIN_FPS - frameTime);
        }
    }

    // Liberando la memoria y cerrando el SDL.
    for (int i = 0; i < numElements; ++i) {
        delete elements[i];
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/**
 * @file ejemploParalelo.cpp
 * @brief Ejemplo de un screensaver paralelo usando SDL2 y OpenMP.
 * @version 0.1
 * @date 2023-08-24
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <cmath>

// Constantes para dimensiones de la ventana.
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

// Constante para el máximo de cuadros por segundo.
const int MIN_FPS = 60;

// Constantes para limitar el número de elementos.
const int MIN_NUM_ELEMENTS = 1;
const int MAX_NUM_ELEMENTS = 100;

// Clase que representa los elementos.
class Element {
public:
    Element(int x, int y, int radius, int speedX, int speedY)
        : x(x), y(y), radius(radius), speedX(speedX), speedY(speedY) {
        // Color aleatorio para cada círculo a generar.
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
    }

    // Actualizador de la posición del elemento.
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
        
        // Dibujando el círculo.
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

    int numElements = 50; // Número predeterminado de elementos

    while (true) {
        // Solicitar el número de elementos al usuario
        std::cout << "Ingresa el número de elementos (entre " << MIN_NUM_ELEMENTS << " y " << MAX_NUM_ELEMENTS << "): ";
        std::string input;
        std::getline(std::cin, input);

        // Si el usuario no ingresó nada, mostrar mensaje de error y volver a pedir
        if (input.empty()) {
            std::cerr << "Error: No se ingresó ningún valor." << std::endl;
        } else {
            // Intentar convertir la entrada a un número
            char* endptr;
            numElements = strtol(input.c_str(), &endptr, 10);

            if (*endptr != '\0' || numElements < MIN_NUM_ELEMENTS || numElements > MAX_NUM_ELEMENTS) {
                std::cerr << "Error: Ingresa un número válido de elementos (entre " << MIN_NUM_ELEMENTS << " y " << MAX_NUM_ELEMENTS << ")." << std::endl;
            } else {
                break; // Salir del bucle si se ingresó un valor válido
            }
        }
    }

    srand(time(nullptr));

    // Creando un arreglo que tiene punteros a elementos.
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

        // Limpieza del renderizador.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Actualización y renderización de los elementos.
        for (int i = 0; i < numElements; ++i) {
            elements[i]->update();
            elements[i]->render(renderer);
        }

        // Presentación del elemento renderizado en la ventana.
        SDL_RenderPresent(renderer);

        // Cálculo e impresión de los FPS de la pantalla.
        frames++;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - prevTime >= 1000) {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
            prevTime = currentTime;
        }

        // Limitación de la velocidad de actualización.
        frameTime = SDL_GetTicks() - startTime;
        if (frameTime < 1000 / MIN_FPS) {
            SDL_Delay(1000 / MIN_FPS - frameTime);
        }
    }

    // Liberación de la memoria y cierre de SDL.
    for (int i = 0; i < numElements; ++i) {
        delete elements[i];
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

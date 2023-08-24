/**
 * @file ejemploPar.cpp
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
#include <thread>
#include <vector>
#include <cmath>

// Propiedades de la pantalla.
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

// Cuadros por segundo.
const int MIN_FPS = 60;

// Clase elemento.
class Element {
public:
    Element(int x, int y, int radius, int speedX, int speedY)
        : x(x), y(y), radius(radius), speedX(speedX), speedY(speedY) {
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
    }

    void update() {
        x += speedX;
        y += speedY;

        // Cambio de direccion de X cuando el elemento choca con alguna "pared" de la pantalla.
        if (x - radius <= 0 || x + radius >= SCREEN_WIDTH) {
            speedX = -speedX;
        }
        
        // Cambio de direccion de Y cuando el elemento choca con alguna "pared" de la pantalla.
        if (y - radius <= 0 || y + radius >= SCREEN_HEIGHT) {
            speedY = -speedY;
        }

        speedX = std::min(std::max(speedX, -5), 5);
        speedY = std::min(std::max(speedY, -5), 5);
    }

    // Renderizador del elemento.
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

// Funcion para actualizar la pos de los elementos en paralelo.
void updateElements(Element** elements, int numElements) {
    for (int i = 0; i < numElements; ++i) {
        elements[i]->update();
    }
}

// Funcion para renderizar los elementos en paralelo.
void renderElements(SDL_Renderer* renderer, Element** elements, int numElements) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < numElements; ++i) {
        elements[i]->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Creando la ventana.
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

    // Obteniendo la cantidad de elementos que ingreso el user, si no, se usan 50 default.
    int numElements = (argc > 1) ? atoi(argv[1]) : 50;
    srand(time(nullptr));

    // Creando un vector de punteros a elementos del programa.
    std::vector<Element*> elements;
    for (int i = 0; i < numElements; ++i) {
        // Generando propiedades aleatorias por cada uno de los elementos creados.
        int x = rand() % (SCREEN_WIDTH - 30) + 30;
        int y = rand() % (SCREEN_HEIGHT - 30) + 30;
        int radius = rand() % 10 + 5;
        int speedX = rand() % 5 + 1;
        int speedY = rand() % 5 + 1;
        elements.push_back(new Element(x, y, radius, speedX, speedY));
    }

    bool quit = false;
    SDL_Event e;
    Uint32 startTime, frameTime, prevTime = 0;
    int frames = 0;

    // Bucle principal del programa.
    while (!quit) {
        startTime = SDL_GetTicks();

        // Manejando los elementos de SDL.
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Creando hilos para poder actualizar y renderizar los elementos que se crearon en paralelo.
        std::thread updateThread(updateElements, elements.data(), numElements);
        std::thread renderThread(renderElements, renderer, elements.data(), numElements);

        // Esperando a que los hilos terminen.
        updateThread.join();
        renderThread.join();

        // Calculando e imprimiendo los FPS del programa.
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
    for (Element* element : elements) {
        delete element;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

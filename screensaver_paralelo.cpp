/**
 * @file ejemploParalelo.cpp
 * @brief Ejemplo de un screensaver paralelo usando SDL2 y OpenMP.
 * @version 0.1
 * @date 2023-08-24
 */


 /*
    Referencias: 
    Paralelismo en pipeline execution con OpenMP: https://stackoverflow.com/questions/44169351/employing-parallelism-in-pipelined-execution
    Uso de taskyield para permitir que otras tareas trabajen en el equipo: https://www.openmp.org/spec-html/5.0/openmpsu49.html

    Puntos extra:
    1. En el private de la clase Element.
    2. Optimizaciòn de memoria compartida en el main. (Son los #pragma omp parallel for shared(elements))
    3. Optimizaciòn de liberaciòn de memoria en el main. (Es el #pragma omp task)
    4. Usando mecanismos de pragma no vistos en clase (el #pragma omp taskyield de los #pragma omp parallel for shared(elements))

  */

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <cmath>
#include <omp.h>

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
} __attribute__((aligned(64))); // Alineación de memoria de 64 bytes


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    int numElements = 0; // Número predeterminado de elementos

    while (true) {
        // Solicitar el número de elementos al usuario
        std::cout << "Ingresa el número de elementos (mayor a 0): ";
        std::string input;
        std::getline(std::cin, input);

        // Si el usuario no ingresó nada, mostrar mensaje de error y volver a pedir
        if (input.empty()) {
            std::cerr << "Error: No se ingresó ningún valor." << std::endl;
        } else {
            // Intentar convertir la entrada a un número
            char* endptr;
            numElements = strtol(input.c_str(), &endptr, 10);

            if (*endptr != '\0' || numElements < MIN_NUM_ELEMENTS) {
                std::cerr << "Error: Ingresa un número válido de elementos (mayor a 0)." << std::endl;
            } else {
                break; // Salir del bucle si se ingresó un valor válido
            }
        }
    }

    srand(time(nullptr));

    // Creando un arreglo que tiene punteros a elementos.
    Element* elements[numElements];
    #pragma omp parallel for num_threads(6) // Crear elementos en paralelo usando OpenMP
    for (int i = 0; i < numElements; ++i) {
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


    // Medición del tiempo secuencial
    double startSec = omp_get_wtime();

    while (!quit) {
        startTime = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < numElements; ++i) {
            elements[i]->update();
            elements[i]->render(renderer);
        }

        SDL_RenderPresent(renderer);

        frames++;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - prevTime >= 1000) {
            std::cout << "FPS (Parallel): " << frames << std::endl;
            frames = 0;
            prevTime = currentTime;
        }

        frameTime = SDL_GetTicks() - startTime;
        if (frameTime < 1000 / MIN_FPS) {
            SDL_Delay(1000 / MIN_FPS - frameTime);
        }
    }

    double endSec = omp_get_wtime(); // Fin de la medición secuencial

    //double startPar = omp_get_wtime(); // Inicio de la medición

    // Reiniciar variables para la medición en paralelo
    quit = false;
    frames = 0;
    prevTime = 0;

    double startPar = omp_get_wtime();

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

        //printf("Max threads: %d\n", omp_get_max_threads());

        #pragma omp parallel for num_threads(6)
        for (int i = 0; i < numElements; ++i) {
            elements[i]->update();
        }

        for (int i = 0; i < numElements; ++i) {
            elements[i]->render(renderer);
        }
        // Presentación del elemento renderizado en la ventana.
        SDL_RenderPresent(renderer);

        frames++;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - prevTime >= 1000) {
            std::cout << "FPS (Parallel): " << frames << std::endl;
            frames = 0;
            prevTime = currentTime;
        }

        frameTime = SDL_GetTicks() - startTime;
        if (frameTime < 1000 / MIN_FPS) {
            SDL_Delay(1000 / MIN_FPS - frameTime);
        }
    }
    double endPar = omp_get_wtime();

    double T_sec = endSec - startSec;
    double T_par = endPar - startPar;
    int P = omp_get_max_threads();
    double speedup = T_sec / T_par;
    double efficiency = speedup / P;

    std::cout << "Tiempo secuencial: " << T_sec << " segundos" << std::endl;
    std::cout << "Tiempo paralelo: " << T_par << " segundos" << std::endl;
    std::cout << "Speedup: " << speedup << std::endl;
    std::cout << "Eficiencia: " << efficiency << std::endl;

    for (int i = 0; i < numElements; ++i) {
        delete elements[i];
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <thread>
#include <vector>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int MIN_FPS = 60;

class Element {
public:
    Element(int x, int y, int size, int speed) : x(x), y(y), size(size), speed(speed) {
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
    }

    void update() {
        x += speed;
        if (x > SCREEN_WIDTH) {
            x = -size;
            y = rand() % SCREEN_HEIGHT;
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_Rect rect = { x, y, size, size };
        SDL_RenderFillRect(renderer, &rect);
    }

private:
    int x, y, size, speed;
    SDL_Color color;
};

void renderLoop(SDL_Renderer* renderer, std::vector<Element*>& elements) {
    bool quit = false;
    SDL_Event e;
    Uint32 startTime, frameTime;
    int frames = 0;

    while (!quit) {
        startTime = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto element : elements) {
            element->update();
            element->render(renderer);
        }

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - startTime;
        if (frameTime < 1000 / MIN_FPS) {
            SDL_Delay(1000 / MIN_FPS - frameTime);
        }

        frames++;
        if (frames % MIN_FPS == 0) {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
        }
    }
}

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

    int numElements = (argc > 1) ? atoi(argv[1]) : 50;
    srand(time(nullptr));

    std::vector<Element*> elements;
    for (int i = 0; i < numElements; ++i) {
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;
        int size = rand() % 20 + 10;
        int speed = rand() % 5 + 1;
        elements.push_back(new Element(x, y, size, speed));
    }

    // Medición del tiempo de ejecución secuencial
    auto startSequential = std::chrono::high_resolution_clock::now();

    for (auto element : elements) {
        element->update();
        element->render(renderer);
    }

    auto endSequential = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sequentialDuration = endSequential - startSequential;

    // Creación y medición del tiempo de ejecución en paralelo
    std::thread renderThread(renderLoop, renderer, std::ref(elements));

    auto startParallel = std::chrono::high_resolution_clock::now();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    auto endParallel = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> parallelDuration = endParallel - startParallel;

    renderThread.join();

    // Cálculo del speedup y la eficiencia
    double speedup = sequentialDuration.count() / parallelDuration.count();
    double efficiency = speedup / std::thread::hardware_concurrency();

    std::cout << "Speedup: " << speedup << std::endl;
    std::cout << "Eficiencia: " << efficiency << std::endl;

    // Liberación de memoria
    for (auto element : elements) {
        delete element;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
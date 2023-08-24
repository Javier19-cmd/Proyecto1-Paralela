#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <thread>
#include <vector>
#include <cmath>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;
const int MIN_FPS = 60;

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

        if (x - radius <= 0 || x + radius >= SCREEN_WIDTH) {
            speedX = -speedX;
        }

        if (y - radius <= 0 || y + radius >= SCREEN_HEIGHT) {
            speedY = -speedY;
        }

        speedX = std::min(std::max(speedX, -5), 5);
        speedY = std::min(std::max(speedY, -5), 5);
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        
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

void updateElements(Element** elements, int numElements) {
    for (int i = 0; i < numElements; ++i) {
        elements[i]->update();
    }
}

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

    while (!quit) {
        startTime = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        std::thread updateThread(updateElements, elements.data(), numElements);
        std::thread renderThread(renderElements, renderer, elements.data(), numElements);

        updateThread.join();
        renderThread.join();

        frames++;
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - prevTime >= 1000) {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
            prevTime = currentTime;
        }

        frameTime = SDL_GetTicks() - startTime;
        if (frameTime < 1000 / MIN_FPS) {
            SDL_Delay(1000 / MIN_FPS - frameTime);
        }
    }

    for (Element* element : elements) {
        delete element;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int STAR_COUNT = 1000;
const int MAX_STAR_SPEED = 3;
const int FPS = 60;

struct Star {
    float x, y, z;
};

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event event;
    bool quit = false;
    std::srand(std::time(nullptr));
    Star stars[STAR_COUNT];
    Uint32 start_time, end_time, frame_time;
    int frames = 0;
    char fps_text[10];

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
        return 1;
    }

    window = SDL_CreateWindow("Screensaver",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    for (int i = 0; i < STAR_COUNT; ++i) {
        stars[i].x = std::rand() % SCREEN_WIDTH - SCREEN_WIDTH / 2;
        stars[i].y = std::rand() % SCREEN_HEIGHT - SCREEN_HEIGHT / 2;
        stars[i].z = std::rand() % SCREEN_WIDTH;
    }

    start_time = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < STAR_COUNT; ++i) {
            stars[i].z -= std::rand() % MAX_STAR_SPEED + 1;

            if (stars[i].z <= 0) {
                stars[i].x = std::rand() % SCREEN_WIDTH - SCREEN_WIDTH / 2;
                stars[i].y = std::rand() % SCREEN_HEIGHT - SCREEN_HEIGHT / 2;
                stars[i].z = SCREEN_WIDTH;
            }

            int x = stars[i].x * SCREEN_WIDTH / stars[i].z + SCREEN_WIDTH / 2;
            int y = stars[i].y * SCREEN_HEIGHT / stars[i].z + SCREEN_HEIGHT / 2;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }

        ++frames;
        end_time = SDL_GetTicks();
        frame_time = end_time - start_time;

        if (frame_time < 1000 / FPS) {
            SDL_Delay(1000 / FPS - frame_time);
            end_time = SDL_GetTicks();
            frame_time = end_time - start_time;
        }

        if (frame_time >= 1000) {
            int fps = frames * 1000 / frame_time;
            printf("FPS: %d\n", fps);
            start_time = end_time;
            frames = 0;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


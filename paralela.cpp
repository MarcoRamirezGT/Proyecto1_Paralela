///Aun en desarrollo

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL.h>
#include <omp.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int STAR_SIZE = 2;
const int MAX_SPEED = 10;
const int MAX_NUM_STARS = 10000;
const int FPS = 60;

struct Star {
    int x;
    int y;
    int speed;
};

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event event;
    bool quit = false;
    std::vector<Star> stars;
    std::srand(std::time(nullptr));
    Uint32 start_time, end_time, frame_time;
    int frames = 0;

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

    // Create initial stars
    for (int i = 0; i < MAX_NUM_STARS; ++i) {
        Star star;
        star.x = std::rand() % SCREEN_WIDTH;
        star.y = std::rand() % SCREEN_HEIGHT;
        star.speed = std::rand() % MAX_SPEED + 1;
        stars.push_back(star);
    }

    start_time = SDL_GetTicks();

#pragma omp parallel
    {
        while (!quit) {
#pragma omp master
            {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quit = true;
                    }
                }
            }

            // Clear screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Update star positions
#pragma omp for
            for (int i = 0; i < stars.size(); ++i) {
                stars[i].y += stars[i].speed;
                if (stars[i].y > SCREEN_HEIGHT) {
                    stars[i].x = std::rand() % SCREEN_WIDTH;
                    stars[i].y = -STAR_SIZE;
                    stars[i].speed = std::rand() % MAX_SPEED + 1;
                }
            }

            // Draw stars
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
#pragma omp for
            for (int i = 0; i < stars.size(); ++i) {
                SDL_Rect rect = { stars[i].x, stars[i].y, STAR_SIZE, STAR_SIZE };
                SDL_RenderFillRect(renderer, &rect);
            }

            // Update screen
            SDL_RenderPresent(renderer);

            // Limit frame rate to 60fps
#pragma omp master
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
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}




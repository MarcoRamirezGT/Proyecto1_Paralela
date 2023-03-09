#include <iostream>
#include <SDL2/SDL.h>

const int WIDTH = 640;
const int HEIGHT = 480;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Screen Saver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect;
    rect.x = WIDTH / 2;
    rect.y = HEIGHT / 2;
    rect.w = 100;
    rect.h = 100;
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    for (int i = 0; i < 100; i++)
    {
        SDL_SetRenderDrawColor(renderer, 60, 169, 186, 255);
        SDL_Rect rect;
        rect.x = (WIDTH / 2) + i * 5;
        rect.y = (HEIGHT / 2) + i * 5;
        rect.w = 100;
        rect.h = 100;
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < HEIGHT; i++)
    {
        SDL_SetRenderDrawColor(renderer, 60, 255, 186, 255);
        SDL_Rect rect;
        rect.x = (i * 10) + 100;
        rect.y = i * 10;
        rect.w = 1 * i + 1;
        rect.h = 1 * i + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }
    if (NULL == window)
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Event windowEvent;

    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (SDL_QUIT == windowEvent.type)
            {
                if (SDL_QUIT == windowEvent.type)
                {
                    break;
                }
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
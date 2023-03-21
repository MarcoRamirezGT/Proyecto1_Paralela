#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL2/SDL.h>
#include <algorithm>
#include <time.h>
#include <math.h>

// Datos de estrellas
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int STAR_SIZE = 1;
const int MAX_SPEED = 100;
const int MAX_NUM_STARS = 100;
const int FPS = 60;
// Datos de cometas
const int COMET_SIZE = 20;
const int MIN_SPEED_COMETS = 100;
const int MAX_SPEED_COMETS = 200;
const int COLLISION_DISTANCE = COMET_SIZE * 2;

// Estructura de estrella
struct Star
{
    int x;
    int y;
    int speed;
};

// Estructura de cometa
struct Comet
{
    int x;
    int y;
    int vx;
    int vy;
};

bool check_collision(const Comet &comet1, const Comet &comet2)
{
    SDL_Rect rect1 = {comet1.x, comet1.y, COMET_SIZE, COMET_SIZE};
    SDL_Rect rect2 = {comet2.x, comet2.y, COMET_SIZE, COMET_SIZE};
    return SDL_HasIntersection(&rect1, &rect2) == SDL_TRUE;
}

void handle_collision(Comet &comet1, Comet &comet2)
{
    int dx = comet1.x - comet2.x;
    int dy = comet1.y - comet2.y;
    int distance_squared = dx * dx + dy * dy;
    if (distance_squared < COLLISION_DISTANCE * COLLISION_DISTANCE)
    {
        // Invertir velocidades perpendiculares al plano de colisión
        if (dx != 0)
        {
            int vx1_perp = comet1.vy * dx / distance_squared;
            int vx2_perp = comet2.vy * dx / distance_squared;
            comet1.vx -= vx1_perp;
            comet2.vx -= vx2_perp;
            comet1.vy += vx1_perp;
            comet2.vy += vx2_perp;
        }
        if (dy != 0)
        {
            int vy1_perp = comet1.vx * dy / distance_squared;
            int vy2_perp = comet2.vx * dy / distance_squared;
            comet1.vy -= vy1_perp;
            comet2.vy -= vy2_perp;
            comet1.vx += vy1_perp;
            comet2.vx += vy2_perp;
        }
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool quit = false;
    std::vector<Star> stars;
    std::vector<Comet> comets;
    std::srand(std::time(nullptr));
    Uint32 start_time, end_time, frame_time;
    Uint32 start_time_comet, end_time_comet, delta_time;

    int frames = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
        return 1;
    }

    window = SDL_CreateWindow("Screensaver",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create initial stars
    for (int i = 0; i < MAX_NUM_STARS; ++i)
    {
        Star star;
        star.x = std::rand() % SCREEN_WIDTH;
        star.y = std::rand() % SCREEN_HEIGHT;
        star.speed = std::rand() % MAX_SPEED + 1;
        stars.push_back(star);
    }
    // Create N comets with random positions and velocities
    int num_comets = 10;
    for (int i = 0; i < num_comets; ++i)
    {
        Comet comet;
        comet.x = std::rand() % (SCREEN_WIDTH - COMET_SIZE);
        comet.y = std::rand() % (SCREEN_HEIGHT - COMET_SIZE);
        comet.vx = std::rand() % (MIN_SPEED_COMETS + 1) + MIN_SPEED_COMETS;
        if (std::rand() % 2 == 0)
        {
            comet.vx = -comet.vx;
        }
        comet.vy = std::rand() % (MIN_SPEED_COMETS + 1) + MIN_SPEED_COMETS;
        if (std::rand() % 2 == 0)
        {
            comet.vy = -comet.vy;
        }
        comets.push_back(comet);
    }
    start_time_comet = SDL_GetTicks();

    start_time = SDL_GetTicks();

    {
        while (!quit)
        {

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
            }

            // Clear screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            //  create 5 comets with random velocities and positions every 5 seconds

            int time_counter = SDL_GetTicks();
            if (time_counter % 300 == 0)
            {
                if (comets.size() < 5)
                {
                    int c = std::rand() % 5;
                    int num_comets = c;
                    for (int i = 0; i < num_comets; ++i)
                    {
                        Comet comet;
                        int angle = std::rand() % 360;
                        angle = angle * 3.14159265 / 180;
                        comet.x = std::rand() % (SCREEN_WIDTH - COMET_SIZE);
                        comet.y = 0;
                        comet.vx = std::rand() % (MAX_SPEED_COMETS - MIN_SPEED_COMETS + 1) + MIN_SPEED_COMETS;
                        comet.vy = std::rand() % (MAX_SPEED_COMETS - MIN_SPEED_COMETS + 1) + MIN_SPEED_COMETS;
                        if (std::rand() % 2 == 0)
                        {
                            comet.vx = comet.vy * tan(angle);
                        }
                        comets.push_back(comet);
                    }
                }
            }

            // Delete comets that have a velocity of 0
            for (auto it = comets.begin(); it != comets.end();)
            {
                if (it->vx == 0 && it->vy == 0)
                {
                    it = comets.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            end_time_comet = SDL_GetTicks();
            delta_time = end_time_comet - start_time_comet;
            start_time_comet = end_time_comet;
            for (auto &comet : comets)
            {
                // check fps and update comet position accordingly to keep speed constant
                comet.x += comet.vx * delta_time / 100;
                comet.y += comet.vy * delta_time / 100;
                // comet.x += comet.vx;
                // comet.y += comet.vy;
                if (comet.x < 0 || comet.x > SCREEN_WIDTH - COMET_SIZE)
                {
                    comet.vx = -comet.vx;
                }
                if (comet.y < 0 || comet.y > SCREEN_HEIGHT - COMET_SIZE)
                {
                    comet.vy = -comet.vy;
                }
            }

            // Check for collisions and handle collided comets
            for (int i = 0; i < comets.size(); ++i)
            {
                for (int j = i + 1; j < comets.size(); ++j)
                {
                    if (check_collision(comets[i], comets[j]))
                    {
                        handle_collision(comets[i], comets[j]);
                    }
                }
            }

            // Remove comets that have gone off-screen
            comets.erase(
                std::remove_if(
                    comets.begin(), comets.end(),
                    [](const Comet &comet)
                    {
                        return comet.x < 0 || comet.x > SCREEN_WIDTH - COMET_SIZE ||
                               comet.y < 0 || comet.y > SCREEN_HEIGHT - COMET_SIZE;
                    }),
                comets.end());

            // Draw comets
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            for (const auto &comet : comets)
            {
                SDL_Rect rect = {comet.x, comet.y, COMET_SIZE, COMET_SIZE};
                SDL_RenderFillRect(renderer, &rect);
            }
            SDL_RenderPresent(renderer);

            // Update star positions

            for (int i = 0; i < stars.size(); ++i)
            {
                stars[i].y += stars[i].speed;
                if (stars[i].y > SCREEN_HEIGHT)
                {
                    stars[i].x = std::rand() % SCREEN_WIDTH;
                    stars[i].y = -STAR_SIZE;
                    stars[i].speed = std::rand() % MAX_SPEED + 1;
                }
            }

            for (int i = 0; i < stars.size(); ++i)
            {
                // Random Color to stars
                // int random = rand() % 255;

                // Draw stars
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                SDL_Rect rect = {stars[i].x, stars[i].y, STAR_SIZE, STAR_SIZE};
                SDL_RenderFillRect(renderer, &rect);
            }

            // Update screen
            SDL_RenderPresent(renderer);

            // Limit frame rate to 60fps

            ++frames;
            end_time = SDL_GetTicks();
            frame_time = end_time - start_time;

            if (frame_time < 1000 / FPS)
            {
                SDL_Delay(1000 / FPS - frame_time);
                end_time = SDL_GetTicks();
                frame_time = end_time - start_time;
            }

            if (frame_time >= 1000)
            {
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
};

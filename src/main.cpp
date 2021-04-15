#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <ctime>
#include <cstdlib>
#include <random>
#include "window/RenderWindow.h"
#include "window/TerminalRender.h"

TTF_Font *font16;
TTF_Font *font24;
TTF_Font *font32;
TTF_Font *font32_outline;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RenderWindow window = RenderWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

bool running;

void game_loop(TerminalRender *terminal);

int a = 0;
int b = 1;

int main(int argc, char *args[]) {

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    window.create("Mountain - the game", 1280, 720);

    font16 = TTF_OpenFont("res/ttf/ModeSeven.TTF", 16);
    font24 = TTF_OpenFont("res/ttf/ModeSeven.TTF", 24);
    font32 = TTF_OpenFont("res/ttf/ModeSeven.TTF", 32);
    font32_outline = TTF_OpenFont("res/ttf/ModeSeven.TTF", 32);

    TTF_SetFontOutline(font32_outline, 3);

    TerminalRender terminal_render = TerminalRender(&window, font24);

    srand((unsigned) time(nullptr));

    running = true;

    window.display();

    while (running) {
        game_loop(&terminal_render);
        SDL_Delay(16);
    }

    window.cleanUp();

    TTF_CloseFont(font32);
    TTF_CloseFont(font32_outline);
    TTF_CloseFont(font24);
    TTF_CloseFont(font16);

    TTF_Quit();
    SDL_Quit();

    return 0;
}

Uint8 rgb_color_code() {

    std::random_device rd;

    std::mt19937 mt(rd());

    std::uniform_int_distribution<int> dist(0, 255);

    return dist(mt);

}

void game_loop(TerminalRender *terminal) {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

    }

    if (a > 60) {
        terminal->write("Cycle - " + std::to_string(b), {rgb_color_code(), rgb_color_code(), rgb_color_code()});
        b++;
        a = 0;
    } else {
        a++;
    }

}

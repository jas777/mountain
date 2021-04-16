#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <ctime>
#include <cstdlib>
#include <random>
#include <iostream>
#include "window/RenderWindow.h"
#include "window/TerminalRender.h"
#include "event/EventHandler.h"

TTF_Font *font16;
TTF_Font *font24;
TTF_Font *font32;
TTF_Font *font32_outline;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RenderWindow window = RenderWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

bool running;

void game_loop(TerminalRender &terminal);

void input(TerminalRender &terminal);

unsigned int frame_count;
unsigned int timerFPS;
unsigned int fps;
unsigned int last_frame;
unsigned int last_time;

int cursor_tick;
int cursor_cycle;

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

    TerminalRender terminal_render = TerminalRender(&window, font16);

    terminal_render.write("[#34D399] MountainOS [#CCC] - use [#6366F1] ? [#CCC] to get information about commands (or quests)", {0xCC, 0xCC, 0xCC});
    // terminal_render.write("Line 2", {0xCC, 0xCC, 0xCC}, 1);

    // SDL_Delay(3000);
    // terminal_render.clear_line(1);
    // terminal_render.write("Line [#2A53B1] colored part [#CCC] not colored part", { 0xCC, 0xCC, 0xCC });

    running = true;

    window.display();

    while (running) {
        input(terminal_render);
        game_loop(terminal_render);
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

//Uint8 rgb_color_code() {
//
//    std::random_device rd;
//
//    std::mt19937 mt(rd());
//
//    std::uniform_int_distribution<int> dist(0, 255);
//
//    return dist(mt);
//
//}

void input(TerminalRender &terminal) {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        EventHandler::handle_event(&event, terminal, running);
    }

}

void game_loop(TerminalRender &terminal) {

    last_frame = SDL_GetTicks();

    if (last_frame >= last_time + 1000) {
        last_time = last_frame;
        fps = frame_count;
        frame_count = 0;
    }

    frame_count++;
    timerFPS = SDL_GetTicks() - last_frame;

    if (timerFPS < (1000 / 300)) {
        SDL_Delay(1000 / 300);
    }

    terminal.refresh_cursor();

    if (cursor_tick >= 140) {
        terminal.draw_cursor(true);
        if (cursor_tick >= 280) {
            cursor_tick = 0;
        }
    } else {
        terminal.draw_cursor(false);
    }

    cursor_tick++;

}

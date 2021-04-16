
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>

#include "RenderWindow.h"

RenderWindow::RenderWindow(int width, int height) {
    this->width = width;
    this->height = height;
};

void RenderWindow::create(const char *p_title, int p_w, int p_h) {
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == nullptr)
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


SDL_Texture *RenderWindow::loadTexture(const char *p_filePath) {
    SDL_Texture *texture = nullptr;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == nullptr)
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

    return texture;
}

void RenderWindow::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void RenderWindow::render(float p_x, float p_y, SDL_Texture *p_tex) {
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    SDL_QueryTexture(p_tex, nullptr, nullptr, &src.w, &src.h);

    SDL_Rect dst;
    dst.x = (int) p_x;
    dst.y = (int) p_y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::render(SDL_Texture *p_tex) {
    render(0, 0, p_tex);
}

void RenderWindow::render(float p_x, float p_y, const char *p_text, TTF_Font *font, SDL_Color textColor) {

    SDL_Surface *surfaceMessage = TTF_RenderUTF8_Blended(font, p_text, textColor);
    SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surfaceMessage->w;
    src.h = surfaceMessage->h;

    SDL_Rect dst;
    dst.x = (int) p_x;
    dst.y = (int) p_y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, message, &src, &dst);
    SDL_FreeSurface(surfaceMessage);
}

void RenderWindow::renderCenter(float p_x, float p_y, const char *p_text, TTF_Font *font, SDL_Color textColor) {
    SDL_Surface *surfaceMessage = TTF_RenderText_Blended(font, p_text, textColor);
    SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surfaceMessage->w;
    src.h = surfaceMessage->h;

    SDL_Rect dst;
    dst.x = this->width / 2 - src.w / 2 + p_x;
    dst.y = this->height / 2 - src.h / 2 + p_y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, message, &src, &dst);
    SDL_FreeSurface(surfaceMessage);
}

void RenderWindow::renderRect(SDL_Rect *rect, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, rect);
    display();
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp() {
    SDL_DestroyWindow(window);
}

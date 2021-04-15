//
// Created by User on 15.04.2021.
//

#ifndef MOUNTAIN_RENDERWINDOW_H
#define MOUNTAIN_RENDERWINDOW_H


#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class RenderWindow {
private:
    SDL_Window *window{};
    SDL_Renderer *renderer{};
public:
    int width;
    int height;

    RenderWindow(int width, int height);

    void create(const char *p_title, int p_w, int p_h);

    SDL_Texture *loadTexture(const char *p_filePath);

    void clear();

    void render(float p_x, float p_y, SDL_Texture *p_tex);

    void render(SDL_Texture *p_tex);

    void render(float p_x, float p_y, const char *p_text, TTF_Font *font, SDL_Color textColor);

    void renderCenter(float p_x, float p_y, const char *p_text, TTF_Font *font, SDL_Color textColor);

    void display();

    void cleanUp();
};


#endif //MOUNTAIN_RENDERWINDOW_H

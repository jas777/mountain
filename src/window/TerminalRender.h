//
// Created by User on 15.04.2021.
//

#ifndef MOUNTAIN_TERMINALRENDER_H
#define MOUNTAIN_TERMINALRENDER_H

#include <string>
#include <vector>
#include "RenderWindow.h"

class TerminalRender {
private:
    RenderWindow *window;
    TTF_Font *font;
    int current_line = 0;
    int lines = 0;
    int line_height;
public:

    std::vector<std::string> linebuf;

    SDL_Color text_color = {0xCC, 0xCC, 0xCC, 0xFF};

    TerminalRender(RenderWindow *window, TTF_Font *font);

    void write(std::string text, SDL_Color color);

    void clear();

    void clear_line(int line);
};


#endif //MOUNTAIN_TERMINALRENDER_H

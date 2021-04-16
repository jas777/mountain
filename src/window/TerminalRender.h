//
// Created by User on 15.04.2021.
//

#ifndef MOUNTAIN_TERMINALRENDER_H
#define MOUNTAIN_TERMINALRENDER_H

#include <string>
#include <vector>
#include "RenderWindow.h"

struct Point {
    int x = 0;
    int y = 0;
};

class TerminalRender {
private:
    RenderWindow *window;
    TTF_Font *font;
    int next_line = 0;
    int input_line = 0;
    int lines = 0;
    int line_height;
    Point cursor_pos;
    bool cursor_shown;
public:

    std::string current_path = "player>";
    std::string input;

    std::vector<std::string> linebuf;

    SDL_Color text_color = {0xCC, 0xCC, 0xCC, 0xFF};

    TerminalRender(RenderWindow *window, TTF_Font *font);

    void write(const std::string& text, SDL_Color color, bool save_buf);
    void write(const std::string& text, SDL_Color color);
    void write(const std::string& text, SDL_Color color, int line, bool save_buf);
    void write(const std::string& text, SDL_Color color, int line);

    void input_set(std::string& text);
    std::string get_input();

    void refresh_cursor();
    void draw_cursor(bool show);
    void draw_cursor(bool show, bool force_update);

    void force_cursor_update();

    void clear();

    void clear_line(int line);

};


#endif //MOUNTAIN_TERMINALRENDER_H

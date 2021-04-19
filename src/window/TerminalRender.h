//
// Created by User on 18.04.2021.
//

#ifndef MOUNTAIN_TERMINALRENDER_H_NEW
#define MOUNTAIN_TERMINALRENDER_H_NEW

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "RenderWindow.h"

struct LineData {
    std::string content_raw;
    std::vector<std::string> parts;
    std::vector<SDL_Color> part_colors;
};

struct Cursor {
    int x;
    int y;
};

class TerminalRender {

private:

    RenderWindow *window;

    TTF_Font *font;
    std::vector<LineData> line_buffer;
    std::string input_raw;
    std::string current_path;

    SDL_Color default_color;

    Cursor cursor;

    int char_size;
    int line_height;
    int current_line;
    int lines;
    int cols;

public:

    TerminalRender(RenderWindow *window, TTF_Font *font, SDL_Color default_color);

    // Input

    void input_add(std::string text);

    void input_set(std::string text);

    std::string get_input();

    // Text rendering

    void write(const std::string &text);

    void write(const std::string &text, int line);

    void clear_line(int line);

    void clear();

    void refresh();

    // Cursor

    void draw_cursor(bool show, bool force_update);

    // Text utilities

    static std::string strip_colors(std::string text);

    std::vector<LineData> parse_text(const std::string &text);

    // Line utilities

    unsigned int calc_lines(const std::string& text) const;

    bool too_long(const std::string &text);

    void shift();

};


#endif //MOUNTAIN_TERMINALRENDER_H_NEW

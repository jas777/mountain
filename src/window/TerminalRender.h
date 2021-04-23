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
    std::u8string content_raw;
    std::vector<std::u8string> parts;
    std::vector<SDL_Color> part_colors;
};

struct Cursor {
    unsigned int x = 0;
    unsigned int y = 0;
};

class TerminalRender {

private:

    RenderWindow *window;

    TTF_Font *font;
    std::vector<LineData> line_buffer;
    std::u8string input_raw;
    std::u8string current_path = u8"some/debug/path";

    SDL_Color default_color;

    Cursor cursor;

    int char_size;
    int line_height;
    int current_line;
    int lines;
    int cols;

    bool cursor_shown = false;

public:

    TerminalRender(RenderWindow *window, TTF_Font *font, SDL_Color default_color);

    // Input

    void input_submit();

    void input_add(std::u8string text);

    void input_set(std::u8string text);

    void input_change(const std::u8string &new_input, const std::u8string &old_input);

    std::u8string get_input();

    // Text rendering

    void write(const std::u8string &text);

    void write(const std::u8string &text, int line, bool cursor_follow);

    void clear_line(int line);

    void clear();

    void refresh();

    // Text utilities

    static std::u8string strip_colors(std::u8string text);

    std::vector<LineData> parse_text(const std::u8string &text);

    static unsigned int text_length(const std::u8string &text);

    static void split_string(std::u8string s, std::vector<std::u8string> &v);

    // Cursor

    void draw_cursor(bool show, bool force_update);

    void move_cursor(unsigned int x, unsigned int y);

    // Line utilities

    [[nodiscard]] unsigned int calc_lines(const std::u8string &text) const;

    bool too_long(const std::u8string &text) const;

    void shift();

};

#endif //MOUNTAIN_TERMINALRENDER_H_NEW

//
// Created by User on 15.04.2021.
//

#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <utility>
#include "TerminalRender.h"
#include "RenderWindow.h"

TerminalRender::TerminalRender(RenderWindow *window, TTF_Font *font) {
    this->window = window;
    this->font = font;

    TTF_SizeText(font, "A", nullptr, &this->line_height);

    this->lines = (this->window->height - 20) / (this->line_height + 3);

    for (int i = 0; i < this->lines; i++) {
        this->linebuf.emplace_back("");
    }

}

void TerminalRender::write(const std::string &text, SDL_Color color, bool save_buf) {

    if (this->lines < this->next_line + 1) {
        this->clear();
        this->next_line = 0;
    }

    write(text, color, this->next_line, save_buf);
    this->next_line++;
    this->input_line++;
}

void TerminalRender::write(const std::string &text, SDL_Color color) {
    write(text, color, true);
}

void TerminalRender::write(const std::string &text, SDL_Color color, int line, bool save_buf) {

    int w, h;

    float y = (float) (line + 1) * ((float) this->line_height + 3);

    std::regex::flag_type regex_flag = std::regex_constants::icase | std::regex_constants::ECMAScript;

    std::regex regexp("\\[((?:#)[0-9a-f]{8}|(?:#)[0-9a-f]{6}|(?:#)[0-9a-f]{4}|(?:#)[0-9a-f]{3})\\]", regex_flag);

    std::smatch matches;

    std::regex_search(text, matches, regexp);

    std::istringstream iss(text);
    std::string s;

    std::vector<std::string> string_parts;
    std::vector<SDL_Color> colors;

    int curr_part = 0;

    string_parts.emplace_back();
    colors.emplace_back(color);

    while (getline(iss, s, ' ')) {
        if (s.empty()) continue;
        if (std::regex_match(s, matches, regexp)) {

            // std::cout << s << std::endl;

            string_parts.emplace_back();
            colors.emplace_back();

            curr_part++;

            std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

            s.replace(0, 1, "").replace(s.length() - 2, 1, "");

            std::smatch match;

            if (std::regex_match(s, match, pattern))
            {

                Uint8 r = std::stoul(match[1].str(), nullptr, 16);
                Uint8 g = std::stoul(match[2].str(), nullptr, 16);
                Uint8 b = std::stoul(match[3].str(), nullptr, 16);

                colors[curr_part] = {r, g, b};
                continue;
            }

            colors[curr_part] = { 0xCC, 0xCC, 0xCC };

            continue;

        }
        string_parts[curr_part] += s + " ";
    }

    int x_add = 0;

    for (int i = 0; i < string_parts.size(); i++) {

        if (i > 0) {

            int part_w = 0;
            TTF_SizeText(font, (string_parts[i - 1]).c_str(), &part_w, nullptr);

            x_add += part_w;

        }

        if (string_parts[i].empty()) continue;

        this->window->render(10 + x_add, y, (string_parts[i]).c_str(),
                             this->font, colors[i]);

    }

    if (save_buf) {
        for (const auto &str : string_parts) {
            this->linebuf[line] += str + (string_parts.size() > 1 ? " " : "");
        }
    }

    this->window->display();

}

void TerminalRender::write(const std::string &text, SDL_Color color, int line) {
    write(text, color, line, true);
}

void TerminalRender::clear() {
    this->window->clear();
}

void TerminalRender::clear_line(int line) {

    int h = 0;

    TTF_SizeText(this->font, "A", nullptr, &h);

    int x = 10;
    float y = (float) (line + 1) * ((float) this->line_height + 3);

    SDL_Rect rect;

    rect.x = x;
    rect.y = y;

    rect.w = this->window->width;
    rect.h = h;

    this->linebuf[line] = "";

    this->window->renderRect(&rect, {0x0, 0x0, 0x0});
    this->window->display();

}

void TerminalRender::input_set(std::string& text) {

}

std::string TerminalRender::get_input() {
    return std::string();
}

void TerminalRender::draw_cursor(bool show) {
    draw_cursor(show, false);
}

void TerminalRender::draw_cursor(bool show, bool force_update) {

    if (show) {
        if (cursor_shown && !force_update) return;
        cursor_shown = true;
        this->clear_line(this->input_line);
        write(this->current_path + " " + this->input + "_", { 0xCC, 0xCC, 0xCC }, this->input_line, false);
    } else {
        if (!cursor_shown && !force_update) return;
        cursor_shown = false;
        this->clear_line(this->input_line);
        write(this->current_path + " " + this->input, { 0xCC, 0xCC, 0xCC }, this->input_line, false);
    }

}

void TerminalRender::refresh_cursor() {
    this->draw_cursor(this->cursor_shown);
}

void TerminalRender::force_cursor_update() {
    draw_cursor(this->cursor_shown, true);
}

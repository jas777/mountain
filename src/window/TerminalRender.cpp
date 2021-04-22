//
// Created by User on 18.04.2021.
//

#include <regex>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <utility>
#include "TerminalRender.h"

TerminalRender::TerminalRender(RenderWindow *window, TTF_Font *font, SDL_Color default_color) {

    this->window = window;
    this->font = font;
    this->default_color = default_color;

    TTF_SizeText(font, "A", &this->char_size, &this->line_height);
    this->line_height += 3;

    // (Assuming the font is monospaced)

    this->cols = (window->width - 16) / this->char_size;
    this->lines = (window->height - 16) / this->line_height;

    for (int i = 0; i < this->lines; i++) {
        this->line_buffer.emplace_back();
    }

}

// Input

void TerminalRender::input_submit() {

    if (this->input_raw.empty()) return;

    this->current_line += calc_lines(strip_colors(this->input_raw));

    this->move_cursor(0, current_line + 1);
    this->input_raw = "";
}

void TerminalRender::input_add(std::string text) {

    input_change(this->input_raw + text, this->input_raw);
    this->input_raw += text;
    this->write(this->input_raw, this->current_line + 1, true);

}

void TerminalRender::input_set(std::string text) {

    input_change(text, this->input_raw);
    this->input_raw = std::move(text);
    this->write(this->input_raw, this->current_line + 1, true);

}

void TerminalRender::input_change(const std::string& new_input, const std::string& old_input) {

    unsigned int new_lines = calc_lines(strip_colors(new_input));
    unsigned int old_lines = calc_lines(strip_colors(old_input));

    if (new_lines < old_lines) {
        for (int i = 1; i <= old_lines - new_lines; i++) {
            clear_line(this->current_line + 1 + i);
        }
    }

}

std::string TerminalRender::get_input() {
    return this->input_raw;
}

// Text rendering

void TerminalRender::write(const std::string &text) {

    unsigned int lines_taken = calc_lines(strip_colors(text));

    if (this->current_line + lines_taken > this->lines) {
        this->shift();
        this->current_line = -1;
    }

    write(text, ++this->current_line, false);

    this->current_line += (signed) lines_taken - 1;

    this->cursor.y = current_line + 1;
    this->cursor.x = this->line_buffer[current_line + 1].content_raw.length() == 0 ? 0 :
                     this->line_buffer[current_line + 1].content_raw.length() + 1;

}

void TerminalRender::write(const std::string &text, int line, bool cursor_follow) {

    int line_count = calc_lines(strip_colors(text));

    for (int i = 0; i < line_count; i++) {
        clear_line(line + i);
    }

    std::vector<LineData> parsed_text = parse_text(text);

    for (int i = 0; i < parsed_text.size(); i++) {

        unsigned int x_offset = 10;
        unsigned int y_offset = ((line + i) * this->line_height) + 10;

        LineData data = parsed_text[i];

        this->line_buffer[line + i] = data;

        for (int j = 0; j < data.parts.size(); j++) {

            if (data.parts[j].empty()) continue;

            this->window->render((float) x_offset, (float) y_offset, data.parts[j].c_str(),
                                 this->font, data.part_colors[j]);

            x_offset += data.parts[j].length() * this->char_size;

        }

    }

    if (cursor_follow) {
        this->cursor.x = this->line_buffer[line + line_count - 1].content_raw.length();
        this->cursor.y = line + line_count - 1;
    }

    this->window->display();

}

void TerminalRender::clear_line(int line) {

    if (line >= this->lines) return;

    int x = 10;
    int y = 10 + (line_height * line);

    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = window->width;
    rect.h = this->line_height - 3;

    this->window->renderRect(&rect, {0x0, 0x0, 0x0});

    line_buffer[line] = LineData{};

}

void TerminalRender::clear() {

}

void TerminalRender::refresh() {

}

// Text utilities

std::string TerminalRender::strip_colors(std::string text) {

    if (text.empty()) return "";

    std::regex::flag_type regex_flag = std::regex_constants::icase | std::regex_constants::ECMAScript;

    std::regex regexp("\\[((?:#)[0-9a-f]{8}|(?:#)[0-9a-f]{6}|(?:#)[0-9a-f]{4}|(?:#)[0-9a-f]{3})\\]", regex_flag);

    std::smatch matches;

    std::regex_search(text, matches, regexp);

    std::istringstream iss(text);
    std::string s;

    int curr_part = 0;

    std::string string_from_parts;

    while (getline(iss, s, ' ')) {

        if (!s.empty() && std::regex_match(s, matches, regexp)) {

            curr_part++;

            std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

            s.replace(0, 1, "").replace(s.length() - 2, 1, "");

            std::smatch match;

            continue;

        }

        string_from_parts += s + " ";

    }

    if (text[text.length() - 1] == ' ') string_from_parts += " ";

    if (!string_from_parts.empty()) string_from_parts.pop_back();

    return string_from_parts;

}

std::vector<LineData> TerminalRender::parse_text(const std::string &text) {

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
    colors.emplace_back(this->default_color);

    while (getline(iss, s, ' ')) {

        if (!s.empty() && std::regex_match(s, matches, regexp)) {

            // std::cout << s << std::endl;

            string_parts.emplace_back();
            colors.emplace_back();

            curr_part++;

            std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

            s.replace(0, 1, "").replace(s.length() - 2, 1, "");

            std::smatch match;

            if (std::regex_match(s, match, pattern)) {

                Uint8 r = std::stoul(match[1].str(), nullptr, 16);
                Uint8 g = std::stoul(match[2].str(), nullptr, 16);
                Uint8 b = std::stoul(match[3].str(), nullptr, 16);

                colors[curr_part] = {r, g, b};
                continue;
            }

            colors[curr_part] = this->default_color;

            continue;

        }

        string_parts[curr_part] += s + " ";

    }

    if (string_parts[curr_part].empty() ||
        string_parts[curr_part][string_parts[curr_part].length() - 2] == ' ')
        string_parts[curr_part] += " ";

    if (!string_parts[curr_part].empty()) string_parts[curr_part].pop_back();

    std::vector<LineData> line_data;

    unsigned int len = 0;

    // Dividing parts into lines

    std::string stripped = strip_colors(text);

    if (calc_lines(stripped) == 1) {
        line_data.emplace_back(LineData{
                stripped,
                string_parts,
                colors
        });
        return line_data;
    }

    int line_index = 0;

    for (int i = 0; i < string_parts.size(); i++) {

        std::string part = string_parts[i];
        SDL_Color part_color = colors[i];

        line_data.emplace_back();

        if (len + part.length() > this->cols) {

            while (len + part.length() > this->cols) {

                line_data.emplace_back();

                std::string sub = part.substr(0, this->cols - len);

                line_data[line_index].parts.emplace_back(sub);
                line_data[line_index].part_colors.emplace_back(part_color);
                line_data[line_index].content_raw += sub;

                part.replace(0, this->cols - len, "");

                len = 0;
                line_index++;

            }

        }

        if (!part.empty()) {
            std::cout << strip_colors(part) << std::endl;
            line_data[line_index].content_raw += strip_colors(part);
            line_data[line_index].parts.emplace_back(part);
            line_data[line_index].part_colors.emplace_back(part_color);
        }

        len += part.length();

    }

    return line_data;

}

// Cursor

void TerminalRender::move_cursor(unsigned int x, unsigned int y) {

    unsigned int old_pos_x = 10 + (this->cursor.x * this->char_size);
    unsigned int old_pos_y = 10 + (line_height * this->cursor.y);

    cursor.x = x;
    cursor.y = y;

    SDL_Rect rect;

    rect.x = (signed) old_pos_x;
    rect.y = (signed) old_pos_y;
    rect.w = char_size;
    rect.h = this->line_height - 3;

    this->window->renderRect(&rect, {0x0, 0x0, 0x0});

    this->window->display();

}

void TerminalRender::draw_cursor(bool show, bool force_update) {

    int x = 10 + (cursor.x * this->char_size);
    int y = 10 + (line_height * cursor.y);

    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = char_size;
    rect.h = this->line_height - 3;

    if (show) {

        if (this->cursor_shown == show && !force_update) return;

        this->window->renderRect(&rect, {0x0, 0x0, 0x0});

//        this->window->render((float) x, (float) y, this->line_buffer[cursor.y].content_raw.length() > cursor.x ?
//                                                   this->line_buffer[cursor.y].content_raw.substr(cursor.x, 1).c_str() :
//                                                   " ", this->font, this->default_color);

        this->window->render((float) x, (float) y, "_", this->font, this->default_color);

        this->cursor_shown = true;

    } else {

        if (this->cursor_shown == show && !force_update) return;

        this->window->renderRect(&rect, {0x0, 0x0, 0x0});

        this->window->render((float) x, (float) y, this->line_buffer[cursor.y].content_raw.length() > cursor.x
                                                   ? this->line_buffer[cursor.y].content_raw.substr(cursor.x, 1).c_str()
                                                   : " ", this->font, this->default_color);

        this->cursor_shown = false;

    }

    this->window->display();

}

// Line utilities

unsigned int TerminalRender::calc_lines(const std::string &text) const {
    return std::ceil((float) text.length() / (float) this->cols);
}

bool TerminalRender::too_long(const std::string &text) {
    return text.length() > this->cols;
}

void TerminalRender::shift() {}

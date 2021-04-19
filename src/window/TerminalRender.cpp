//
// Created by User on 18.04.2021.
//

#include <regex>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
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

}

// Input

void TerminalRender::input_add(std::string text) {

}

void TerminalRender::input_set(std::string text) {

}

std::string TerminalRender::get_input() {
    return this->input_raw;
}

// Text rendering

void TerminalRender::write(const std::string &text) {

    if (this->current_line + calc_lines(text) > this->lines) {
        this->shift();
        this->current_line = -1;
    }

    write(text, this->current_line++);

}

void TerminalRender::write(const std::string &text, int line) {

    std::vector<LineData> parsed_text = parse_text(text);

    for (int i = 0; i < parsed_text.size(); i++) {

        int x_offset = 10;
        int y_offset = (((line + 1) + i) * this->line_height) + 10;

        LineData data = parsed_text[i];

        for (int j = 0; j < data.parts.size(); j++) {

            if (data.parts[j].empty()) continue;

            int w = 0;
            TTF_SizeText(this->font, data.parts[j].c_str(), &w, nullptr);

            this->window->render((float) x_offset, (float) y_offset, data.parts[j].c_str(),
                                 this->font, data.part_colors[j]);

            x_offset += w;

        }

    }

}

void TerminalRender::clear_line(int line) {

}

void TerminalRender::clear() {

}

void TerminalRender::refresh() {

}

// Text utilities

std::string TerminalRender::strip_colors(std::string text) {

    std::regex::flag_type regex_flag = std::regex_constants::icase | std::regex_constants::ECMAScript;

    std::regex regexp("\\[((?:#)[0-9a-f]{8}|(?:#)[0-9a-f]{6}|(?:#)[0-9a-f]{4}|(?:#)[0-9a-f]{3})\\]", regex_flag);

    std::smatch matches;

    std::regex_search(text, matches, regexp);

    std::istringstream iss(text);
    std::string s;

    int curr_part = 0;

    std::string string_from_parts;

    while (getline(iss, s, ' ')) {
        if (s.empty()) continue;
        if (std::regex_match(s, matches, regexp)) {

            curr_part++;

            std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

            s.replace(0, 1, "").replace(s.length() - 2, 1, "");

            std::smatch match;

            continue;

        }

        string_from_parts += s + " ";

    }

    string_from_parts.pop_back();
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

        if (s.empty()) continue;

        if (std::regex_match(s, matches, regexp)) {

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

    string_parts[curr_part].pop_back();

    std::vector<LineData> line_data;

    unsigned int len = 0;

    // Dividing parts into lines

    std::string stripped = strip_colors(text);

    if (calc_lines(stripped) == 1) {
        line_data.emplace_back(LineData{
                text,
                string_parts,
                colors
        });
        return line_data;
    }

    std::cout << "a";

    int line_index = 0;

    for (int i = 0; i < string_parts.size(); i++) {

        std::string part = string_parts[i];
        SDL_Color part_color = colors[i];

        line_data.emplace_back();

        if (len + part.length() - (string_parts.size() == 1 ? 0 : 1) > this->cols) {

            while (len + part.length() - (string_parts.size() == 1 ? 0 : 1) > this->cols) {

                line_data.emplace_back();
                line_data[line_index].parts.emplace_back(part.substr(0, this->cols - len - 1));
                line_data[line_index].part_colors.emplace_back(part_color);
                part.replace(0, this->cols - len, "");

                len = 0;
                line_index++;

            }

        }

        if (!part.empty()) {
            line_data[line_index].parts.emplace_back(part);
            line_data[line_index].part_colors.emplace_back(part_color);
        }

        len += part.length();

    }

    return line_data;

}

// Cursor

void TerminalRender::draw_cursor(bool show, bool force_update) {
    return;
}

// Line utilities

unsigned int TerminalRender::calc_lines(const std::string &text) const {
    return std::ceil((float) text.length() / (float) this->cols);
}

bool TerminalRender::too_long(const std::string &text) {
    return text.length() > this->cols;
}

void TerminalRender::shift() {}
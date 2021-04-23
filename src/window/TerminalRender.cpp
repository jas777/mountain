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
#include "utf8.h"

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
    this->input_raw = u8"";
}

void TerminalRender::input_add(std::u8string text) {

    input_change(this->input_raw + text, this->input_raw);
    this->input_raw += text;
    this->write(this->input_raw, this->current_line + 1, true);

}

void TerminalRender::input_set(std::u8string text) {

    input_change(text, this->input_raw);
    this->input_raw = text_length(text) == 0 ? u8"" : std::move(text);
    this->write(this->input_raw, this->current_line + 1, true);

}

void TerminalRender::input_change(const std::u8string &new_input, const std::u8string &old_input) {

    unsigned int new_lines = calc_lines(strip_colors(new_input));
    unsigned int old_lines = calc_lines(strip_colors(old_input));

    if (new_lines < old_lines) {
        for (int i = 1; i <= old_lines - new_lines; i++) {
            clear_line(this->current_line + 1 + i);
        }
    }

}

std::u8string TerminalRender::get_input() {
    return this->input_raw;
}

// Text rendering

void TerminalRender::write(const std::u8string &text) {

    unsigned int lines_taken = calc_lines(strip_colors(text));

    if (this->current_line + lines_taken > this->lines) {
        this->shift();
        this->current_line = -1;
    }

    write(text, ++this->current_line, false);

    this->current_line += (signed) lines_taken - 1;

    this->cursor.y = current_line + 1;
    this->cursor.x = text_length(this->line_buffer[current_line + 1].content_raw) == 0 ? 0 :
                     text_length(this->line_buffer[current_line + 1].content_raw);

}

void TerminalRender::write(const std::u8string &text, int line, bool cursor_follow) {

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

            x_offset += text_length(data.parts[j]) * this->char_size;

        }

    }

    if (cursor_follow) {
        if (line_count > 0) {
            this->cursor.x = text_length(this->line_buffer[line + line_count - 1].content_raw);
            this->cursor.y = line + line_count - 1;
        } else {
            this->move_cursor(0, this->cursor.y);
            draw_cursor(true, true);
        }
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

std::u8string TerminalRender::strip_colors(std::u8string text) {

    if (text_length(text) == 0) return u8"";

    std::regex::flag_type regex_flag = std::regex_constants::icase | std::regex_constants::ECMAScript;

    std::regex regexp("\\[((?:#)[0-9a-f]{8}|(?:#)[0-9a-f]{6}|(?:#)[0-9a-f]{4}|(?:#)[0-9a-f]{3})\\]", regex_flag);

    std::smatch matches;

    std::string converted = std::string(text.begin(), text.end());

    std::regex_search(converted, matches, regexp);

    std::istringstream iss(converted);
    std::string s;

    int curr_part = 0;

    std::u8string string_from_parts;

    while (getline(iss, s, ' ')) {

        if (!s.empty() && std::regex_match(s, matches, regexp)) {

            curr_part++;

            std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

            s.replace(0, 1, "").replace(text_length(std::u8string(s.begin(), s.end())) - 1, 1, "");

            std::smatch match;

            continue;

        }

        string_from_parts += std::u8string(s.begin(), s.end()) + u8" ";

    }

    if (text[text_length(text) - 1] == ' ') string_from_parts += u8" ";

    if (text_length(string_from_parts) != 0) string_from_parts.pop_back();

    return string_from_parts;

}

std::vector<LineData> TerminalRender::parse_text(const std::u8string &text) {

    std::regex::flag_type regex_flag = std::regex_constants::icase | std::regex_constants::ECMAScript;

    std::regex regexp("\\[((?:#)[0-9a-f]{8}|(?:#)[0-9a-f]{6}|(?:#)[0-9a-f]{4}|(?:#)[0-9a-f]{3})\\]", regex_flag);

    std::smatch matches;

    std::string converted = std::string(text.begin(), text.end());

    std::regex_search(converted, matches, regexp);

    std::vector<std::u8string> string_parts;
    std::vector<SDL_Color> colors;

    int curr_part = 0;

    string_parts.emplace_back();
    colors.emplace_back(this->default_color);

    std::vector<std::u8string> _split_string;
    split_string(std::u8string(converted.begin(), converted.end()), _split_string);

    for (auto & s : _split_string) {

        std::string _temp = std::string(s.begin(), s.end());
        if (!s.empty() && std::regex_match(_temp, matches, regexp)) {

            // std::cout << s << std::endl;

            string_parts.emplace_back();
            colors.emplace_back();

            curr_part++;

            std::regex pattern("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");

            _temp.replace(0, 1, "").replace(text_length(std::u8string(_temp.begin(), _temp.end())) - 1, 1, "");

            std::smatch match;

            colors[curr_part] = this->default_color;

            if (std::regex_match(_temp, match, pattern)) {

                Uint8 r = std::stoul(match[1].str(), nullptr, 16);
                Uint8 g = std::stoul(match[2].str(), nullptr, 16);
                Uint8 b = std::stoul(match[3].str(), nullptr, 16);

                colors[curr_part] = {r, g, b};
                continue;
            }

            continue;

        }

        string_parts[curr_part] += std::u8string(_temp.begin(), _temp.end()) + u8" ";

    }

    if (text_length(string_parts[curr_part]) == 0 ||
        string_parts[curr_part][text_length(string_parts[curr_part]) - 1] == ' ')
        string_parts[curr_part] += u8" ";

    if (!string_parts[curr_part].empty()) string_parts[curr_part].pop_back();

    std::vector<LineData> line_data;

    unsigned int len = 0;

    // Dividing parts into lines

    std::u8string stripped = strip_colors(text);

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

        std::u8string part = string_parts[i];
        SDL_Color part_color = colors[i];

        line_data.emplace_back();

        if (len + text_length(part) > this->cols) {

            while (len + text_length(part) > this->cols) {

                line_data.emplace_back();

                std::u8string sub = part.substr(0, this->cols - len);

                line_data[line_index].parts.emplace_back(sub);
                line_data[line_index].part_colors.emplace_back(part_color);
                line_data[line_index].content_raw += sub;

                part.replace(0, this->cols - len, u8"");

                len = 0;
                line_index++;

            }

        }

        if (!part.empty()) {
            line_data[line_index].content_raw += strip_colors(part);
            line_data[line_index].parts.emplace_back(part);
            line_data[line_index].part_colors.emplace_back(part_color);
        }

        len += text_length(part);

    }

    return line_data;

}

void TerminalRender::split_string(std::u8string s, std::vector<std::u8string> &v) {

    std::u8string temp;

    for (char8_t i : s) {

        if (i == ' ') {
            v.push_back(temp);
            temp = u8"";
        } else {
            temp.push_back(i);
        }

    }

    v.push_back(temp);

}

unsigned int TerminalRender::text_length(const std::u8string &text) {
    unsigned int count = 0;
    for (auto &c : text)
        if ((c & 0b1100'0000) != 0b1000'0000) // Not a trailing byte
            ++count;
    return count;
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

        this->window->render((float) x, (float) y, u8"_", this->font, this->default_color);

        this->cursor_shown = true;

    } else {

        if (this->cursor_shown == show && !force_update) return;

        this->window->renderRect(&rect, {0x0, 0x0, 0x0});

        this->window->render((float) x, (float) y, text_length(this->line_buffer[cursor.y].content_raw) > cursor.x
                                                   ? this->line_buffer[cursor.y].content_raw.substr(cursor.x, 1).c_str()
                                                   : u8" ", this->font, this->default_color);

        this->cursor_shown = false;

    }

    this->window->display();

}

// Line utilities

unsigned int TerminalRender::calc_lines(const std::u8string &text) const {
    return std::ceil((float) text_length(text) / (float) cols);
}

bool TerminalRender::too_long(const std::u8string &text) const {
    return text_length(text) > cols;
}

void TerminalRender::shift() {}

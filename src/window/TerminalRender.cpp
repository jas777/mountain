//
// Created by User on 15.04.2021.
//

#include <iostream>
#include "TerminalRender.h"
#include "RenderWindow.h"

TerminalRender::TerminalRender(RenderWindow *window, TTF_Font *font) {
    this->window = window;
    this->font = font;

    TTF_SizeText(font, "A", nullptr, &this->line_height);

    this->lines = (this->window->height - 20) / (this->line_height / 2);

}

void TerminalRender::write(std::string text, SDL_Color color) {

    int w, h;

    TTF_SizeText(font, "A", &w, &h);

    std::cout << w << std::endl;

    TTF_SizeText(font, "A|", &w, &h);

    std::cout << w << std::endl;

    if (this->lines < this->current_line + 1) {
        this->clear();
        this->current_line = 0;
    }

    float y = (float) ++this->current_line * ((float) this->line_height / 2);

    this->window->render(10, y, text.c_str(),
                         this->font, color);

    this->window->display();
    this->current_line++;
}

void TerminalRender::clear() {
    this->window->clear();
}

void TerminalRender::clear_line(int line) {

}

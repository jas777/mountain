//
// Created by User on 15.04.2021.
//

#include <string>
#include <iostream>
#include "EventHandler.h"
#include "../window/TerminalRender.h"

void EventHandler::handle_event(SDL_Event *event, TerminalRender *terminal, bool &running) {

    switch (event->type) {

        case SDL_QUIT:
            running = false;
            break;

        case SDL_TEXTINPUT:

            if (!(SDL_GetModState() & KMOD_CTRL &&
                  (event->text.text[0] == 'c' || event->text.text[0] == 'C' || event->text.text[0] == 'v' ||
                   event->text.text[0] == 'V'))) {
                std::string to_add = event->text.text;
                terminal->input_add(std::u8string(to_add.begin(), to_add.end()));
            } else if (event->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                SDL_SetClipboardText((const char*) terminal->get_input().c_str());
            } else if (event->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                terminal->input_add((const char8_t *) SDL_GetClipboardText());
            }

            break;

        case SDL_KEYDOWN:

            std::u8string input = terminal->get_input();

            switch (event->key.keysym.sym) {
                case SDLK_BACKSPACE:

                    if (TerminalRender::text_length(input) < 1) break;

                    input.pop_back();
                    terminal->input_set(input);

                    break;

                case SDLK_RETURN:

                    if (!terminal->get_input().empty()) {
                        this->input_handler.handle_input(terminal->get_input(), terminal);
                    }

                    terminal->input_submit();

                    break;
            }

    }

}
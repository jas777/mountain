//
// Created by User on 15.04.2021.
//

#include "EventHandler.h"
#include "../window/TerminalRender.h"

void EventHandler::handle_event(SDL_Event *event, TerminalRender &terminal, bool &running) {

    switch (event->type) {

        case SDL_QUIT:
            running = false;
            break;

        case SDL_TEXTINPUT:

            if (!(SDL_GetModState() & KMOD_CTRL &&
                  (event->text.text[0] == 'c' || event->text.text[0] == 'C' || event->text.text[0] == 'v' ||
                   event->text.text[0] == 'V'))) {
                terminal.input += event->text.text;
                terminal.force_cursor_update();
            } else if (event->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                SDL_SetClipboardText(terminal.get_input().c_str());
            } else if (event->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                terminal.input += SDL_GetClipboardText();
                terminal.force_cursor_update();
            }

            break;

        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_BACKSPACE && !terminal.input.empty()) {
                terminal.input.pop_back();
                terminal.force_cursor_update();
            }
            break;

    }

}
//
// Created by User on 15.04.2021.
//

#ifndef MOUNTAIN_EVENTHANDLER_H
#define MOUNTAIN_EVENTHANDLER_H


#include <SDL.h>
#include "../window/TerminalRender.h"

class EventHandler {
public:
    static void handle_event(SDL_Event *event, TerminalRender& terminal, bool& running);
};


#endif //MOUNTAIN_EVENTHANDLER_H

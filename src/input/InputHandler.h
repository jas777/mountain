//
// Created by User on 22.04.2021.
//

#ifndef MOUNTAIN_INPUTHANDLER_H
#define MOUNTAIN_INPUTHANDLER_H

#include <string>
#include "../window/TerminalRender.h"

class InputHandler {
public:
    void handle_input(std::string input, TerminalRender *terminal);
};


#endif //MOUNTAIN_INPUTHANDLER_H

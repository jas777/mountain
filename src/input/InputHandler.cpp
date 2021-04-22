//
// Created by User on 22.04.2021.
//

#include <iostream>
#include "InputHandler.h"

void InputHandler::handle_input(std::string input, TerminalRender *terminal) {

    std::cout << TerminalRender::strip_colors(input) << std::endl;

}

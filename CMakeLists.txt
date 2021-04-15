cmake_minimum_required(VERSION 3.19)
project(mountain)

set(CMAKE_CXX_STANDARD 14)

include_directories(extern/SDL2/SDL/include extern/SDL2/SDL_ttf/include extern/SDL2/SDL_image/include)
link_directories(extern/SDL2/SDL/lib/x86 extern/SDL2/SDL_ttf/lib/x86 extern/SDL2/SDL_image/lib/x86)

add_executable(mountain src/main.cpp src/player/Player.cpp src/player/Player.h src/window/RenderWindow.cpp src/window/RenderWindow.h src/window/TerminalRender.cpp src/window/TerminalRender.h src/event/EventHandler.cpp src/event/EventHandler.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf SDL2_image)

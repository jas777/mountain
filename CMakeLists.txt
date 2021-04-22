cmake_minimum_required(VERSION 3.19)
project(mountain)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)

if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

include_directories(extern/SDL2/SDL/include extern/SDL2/SDL_ttf/include extern/SDL2/SDL_image/include)
link_directories(extern/SDL2/SDL/lib/x86 extern/SDL2/SDL_ttf/lib/x86 extern/SDL2/SDL_image/lib/x86)

add_executable(mountain src/main.cpp src/player/Player.cpp src/player/Player.h src/window/RenderWindow.cpp src/window/RenderWindow.h src/window/TerminalRender.cpp src/window/TerminalRender.h src/event/EventHandler.cpp src/event/EventHandler.h src/input/InputHandler.cpp src/input/InputHandler.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_ttf SDL2_image nlohmann_json::nlohmann_json)

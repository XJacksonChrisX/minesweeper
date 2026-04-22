cmake_minimum_required(VERSION 3.10)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# 源文件
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/board.cpp
    src/cell.cpp
    src/display.cpp
)

# 創建可執行文件
add_executable(minesweeper ${SOURCES})

cmake_minimum_required(VERSION 3.10)

project(Sudoku-C C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(Sudoku-C src/main.c src/util.c src/game.c src/sudoku.c src/io.c)

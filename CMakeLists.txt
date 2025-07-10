cmake_minimum_required(VERSION 3.31)
project(cambio C)

set(CMAKE_C_STANDARD 11)

add_executable(cambio main.c
        cambio.c
        cambio.h
        card.c
        card.h
        action.c
        action.h
        mcts_node.c
        mcts_node.h
)

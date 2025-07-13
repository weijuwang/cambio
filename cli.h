//
// Created by Weiju Wang on 7/11/25.
//

#ifndef CLI_H
#define CLI_H

#include "mcts_node.h"

/**
 * Information about a command.
 */
const struct command_info {
    char* name;
    char* command;
} COMMAND_INFO[500];

/**
 * Print information about a cambio state.
 */
void cambio_print(const struct cambio*);

/**
 * Recursively prints an MCTS node in JSON.
 */
void mcts_print(const struct mcts_node*);

#endif //CLI_H

//
// Created by Weiju Wang on 7/9/25.
//

#ifndef MCTS_NODE_H
#define MCTS_NODE_H

#include "state.h"

/**
 * A node in open-loop MCTS. Note that this does not ever store the game state.
 */
struct mcts_node {
    /**
     * The number of playouts that have been completed from this node.
     */
    unsigned int playouts;

    /**
     * The number of playouts from this node in which the player who did the action won.
     */
    unsigned int wins;

    /**
     * Child nodes.
     */
    struct mcts_node* children[NUM_ACTIONS];
};

/**
 * Initializes the fields in an [mcts_node].
 * @return A pointer to the initialized [mcts_node].
 */
struct mcts_node* mcts_init(struct mcts_node*);

/**
 * Dynamically allocates and initializes a new [mcts_node].
 * @return The new [mcts_node], or `NULL` if it was unsuccessful.
 */
struct mcts_node* mcts_new(void);

/**
 * Recursively frees a dynamically allocated [mcts_node] and all nested children to an indefinite depth.
 */
void mcts_free(struct mcts_node*);

#endif //MCTS_NODE_H

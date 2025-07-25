//
// Created by Weiju Wang on 7/9/25.
//

#ifndef MCTS_NODE_H
#define MCTS_NODE_H

#include "action.h"

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

    /**
     * The parent of this node. `NULL` if this is the root node.
     */
    struct mcts_node* parent;
};

/**
 * Initializes the fields in an [mcts_node].
 * @param parent The parent of the initialized node. `NULL` if this is the root node.
 * @return A pointer to the initialized [mcts_node].
 */
struct mcts_node* mcts_init(struct mcts_node*, const struct mcts_node* parent);

/**
 * Dynamically allocates and initializes a new [mcts_node].
 * @param parent The parent of the initialized node. `NULL` if this is the root node.
 * @return The new [mcts_node], or `NULL` if it was unsuccessful.
 */
struct mcts_node* mcts_new(const struct mcts_node* parent);

/**
 * Recursively frees a dynamically allocated [mcts_node] and all nested children to an indefinite depth.
 */
void mcts_free(struct mcts_node*);

#endif //MCTS_NODE_H

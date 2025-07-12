//
// Created by Weiju Wang on 7/9/25.
//

#include "mcts_node.h"

#include <stdlib.h>

struct mcts_node* mcts_init(struct mcts_node* m, const struct mcts_node* parent) {
    m->playouts = 0;
    m->wins = 0;
    for (enum action i = 0; i < NUM_ACTIONS; ++i)
        m->children[i] = NULL;
    m->parent = parent;
    return m;
}

struct mcts_node* mcts_new(const struct mcts_node* parent) {
    struct mcts_node* m = malloc(sizeof *m);
    return m == NULL ? NULL : mcts_init(m, parent);
}

void mcts_free(struct mcts_node* m) {
    for (enum action i = 0; i < NUM_ACTIONS; ++i)
        if (m->children[i] != NULL)
            mcts_free(m->children[i]);
    free(m);
}
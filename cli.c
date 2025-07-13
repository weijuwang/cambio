//
// Created by Weiju Wang on 7/11/25.
//

#include "cli.h"

#include <stdio.h>

const struct command_info COMMAND_INFO[] = {
    [DRAW] = { "Draw", "+"},
    [DISCARD] = {"Discard", "-"},
    [SWAP] = {"Swap", "x"},
    [BLIND_SWITCH] = {"Blind switch", "s"},
    [PEEK_OWN] = {"Peek own", "."},
    [PEEK_OTHER] = {"Peek other", "."},
    [B_KING_PEEK] = {"Black king peek", "."},
    [STICK] = {"Stick", "!"},
    [GIVE_AWAY] = {"Give away", "."},
    [CAMBIO] = {"Cambio", "cambio"}
};

void cambio_print(const struct cambio* c) {
    printf("turn %i/%i, cambio %i, %i to draw, %s drawn, %s disc'd. ", c->turn, c->num_players, c->cambio_caller,
           c->draw_pile_size, CARD_INFO[c->drawn_card].shorthand, CARD_INFO[c->last_discarded].shorthand);
    for (enum action i = 0; i < NUM_ACTIONS; ++i) {
        if (c->legal_actions & 1<<i)
            printf("%s(%s) ", COMMAND_INFO[i].name, COMMAND_INFO[i].command);
    }
    putchar('\n');
    for (enum player p = 0; p < c->num_players; ++p) {
        putchar('[');
        for (int i = 0; i < PLAYER_MAX_CARDS; ++i) {
            printf("%s ", CARD_INFO[c->player_cards[p][i]].shorthand);
        }
        printf("] ");
    }
    putchar('\n');
}

void mcts_print(const struct mcts_node* m) {
    if (m == NULL) {
        printf("null");
        return;
    }

    printf("{\"playouts\":%i,\"wins\":%i,\"children\":{", m->playouts, m->wins);
    for (enum action i = 0; i < NUM_ACTIONS; ++i) {
        printf("\"%s\":", COMMAND_INFO[i].name);
        mcts_print(m->children[i]);
        if (i != NUM_ACTIONS - 1)
            putchar(',');
    }
    puts("}}");
}
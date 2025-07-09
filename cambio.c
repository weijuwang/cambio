//
// Created by Weiju Wang on 7/8/25.
//

#include "cambio.h"

#include <stdlib.h>
#include <string.h>

const unsigned int START_NUM_CARDS = 4;

struct cambio* cambio_init(struct cambio* c, unsigned int num_players, unsigned int first_player, enum card bottom_left, enum card bottom_right, bool jokers) {
    c->num_players = num_players;
    c->turn = first_player;
    c->cambio_caller = -1;

    /* Set up the frequency table of unseen cards */

    // A-10 each appear 4 times
    for (int i = 1; i < 11; ++i)
        c->unseen_freq_table[i] = 4;
    // 8 face cards: 4 jacks and 4 queens
    c->unseen_freq_table[FACE_CARD] = 8;
    // 2 black kings
    c->unseen_freq_table[B_KING] = 2;
    // 2 red kings
    c->unseen_freq_table[R_KING] = 2;
    // 2 jokers if jokers are being used
    c->unseen_freq_table[JOKER] = jokers ? 2 : 0;

    /* Allocate and set `player_cards` */

    // Allocate the memory
    c->player_cards = malloc(num_players * sizeof(uint8_t[PLAYER_MAX_CARDS]));
    if (c->player_cards == NULL)
        return NULL;

    // For each player
    for (int p = 0; p < num_players; ++p) {
        // All players start with four cards, but they're all face down to us
        for (unsigned int i = 0; i < START_NUM_CARDS; ++i)
            c->player_cards[p][i] = UNKNOWN_CARD;
        // The rest of the cards are null -- players start with just four cards
        for (unsigned int i = START_NUM_CARDS; i < PLAYER_MAX_CARDS; ++i)
            c->player_cards[p][i] = NULL_CARD;
    }

    /* Add information from known cards */

    // Remove the cards from the frequency table of unseen cards
    -- c->unseen_freq_table[bottom_left];
    -- c->unseen_freq_table[bottom_right];

    // Add the cards as our third and fourth cards
    c->player_cards[0][2] = bottom_left;
    c->player_cards[0][3] = bottom_right;

    return c;
}

struct cambio* cambio_new(unsigned int num_players, unsigned int first_player, enum card bottom_left, enum card bottom_right, bool jokers) {
    struct cambio* new_game = malloc(sizeof(struct cambio));
    if (new_game == NULL)
        return NULL;
    return cambio_init(new_game, num_players, first_player, bottom_left, bottom_right, jokers);
}

void cambio_cleanup(struct cambio* c) {
    free(c->player_cards);
}

void cambio_free(struct cambio* c) {
    cambio_cleanup(c);
    free(c);
}
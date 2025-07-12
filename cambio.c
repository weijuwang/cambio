//
// Created by Weiju Wang on 7/8/25.
//

#include "cambio.h"

#include <stdlib.h>
#include <string.h>

const unsigned int START_NUM_CARDS = 4;

struct cambio* cambio_init(struct cambio* c, const unsigned int num_players, const enum player first_player, const enum card bottom_left, const enum card bottom_right, const bool jokers) {
    const unsigned int num_jokers = jokers ? 2 : 0;

    c->num_players = num_players;
    c->turn = first_player - 1;
    c->cambio_caller = NO_PLAYER;
    c->legal_actions = 1<<DRAW | 1<<CAMBIO;
    c->drawn_card = NULL_CARD;
    c->last_discarded = NULL_CARD;

    /* Set up the frequency table of unseen cards */

    // A-10 each appear 4 times
    for (int i = 1; i < 11; ++i)
        c->unseen_ftable[i] = 4;

    c->unseen_ftable[JACK] = 4;
    c->unseen_ftable[QUEEN] = 4;
    c->unseen_ftable[B_KING] = 2;
    c->unseen_ftable[R_KING] = 2;
    c->unseen_ftable[JOKER] = num_jokers;

    c->draw_pile_size = card_ftable_count(c->unseen_ftable) - 4 * num_players;

    /* Zero the discard pile freq table */
    memset(c->discard_ftable, 0, sizeof(card_ftable));

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

    cambio_peek_own(c, 2, bottom_left);
    cambio_peek_own(c, 3, bottom_right);

    return c;
}

struct cambio* cambio_new(const unsigned int num_players, const enum player first_player, const enum card bottom_left, const enum card bottom_right, const bool jokers) {
    struct cambio* new_game = malloc(sizeof(struct cambio));
    if (new_game == NULL)
        return NULL;
    return cambio_init(new_game, num_players, first_player, bottom_left, bottom_right, jokers);
}

struct cambio* cambio_deepcopy(const struct cambio* c) {
    struct cambio* copy = malloc(sizeof(struct cambio));
    if (copy == NULL)
        return NULL;

    memcpy(copy, c, sizeof(struct cambio));

    const unsigned int player_cards_size = c->num_players * sizeof(uint8_t[PLAYER_MAX_CARDS]);
    copy->player_cards = malloc(player_cards_size);
    if (copy->player_cards == NULL)
        return NULL;

    memcpy(copy->player_cards, c->player_cards, player_cards_size);

    return copy;
}

void cambio_cleanup(const struct cambio* c) {
    free(c->player_cards);
}

void cambio_free(struct cambio* c) {
    cambio_cleanup(c);
    free(c);
}

void cambio_inc_turn(struct cambio* c) {
    if (++c->turn == c->num_players)
        c->turn == 0;
}

void cambio_add_legal_action(struct cambio* c, const enum action a) {
    c->legal_actions |= 1<<a;
}

void cambio_remove_legal_action(struct cambio* c, const enum action a) {
    c->legal_actions &= ~(1<<a);
}

void cambio_remove_from_unseen(struct cambio* c, const enum card card) {
    if (card != UNKNOWN_CARD)
        -- c->unseen_ftable[card];
}

void cambio_peek(struct cambio* c, const enum player p, const unsigned int position, const enum card card) {
    cambio_remove_from_unseen(c, card);
    c->player_cards[p][position] = card;
}

void cambio_peek_own(struct cambio* c, const unsigned int position, const enum card card) {
    cambio_peek(c, 0, position, card);
}

void cambio_do_action(struct cambio* c, const enum action a, const int arg0, const int arg1, const int arg2, const int arg3) {
    switch (a) {
        case DRAW:
            cambio_inc_turn(c);

            if (c->turn == c->cambio_caller) {
                // Game is over
                c->legal_actions = 0;
                break;
            }

            // If the draw pile is empty
            if (c->draw_pile_size == 0) {
                // Put all cards from the discard pile into the draw pile, and clear the discard pile
                for (enum card i = 1; i < NUM_CARD_TYPES; ++i) {
                    const unsigned int num_discarded_of_this_type = c->discard_ftable[i];
                    c->unseen_ftable[i] += num_discarded_of_this_type;
                    c->draw_pile_size += num_discarded_of_this_type;
                    c->discard_ftable[i] = 0;
                }
            }

            c->drawn_card = arg0;
            -- c->draw_pile_size;
            cambio_remove_from_unseen(c, arg0);

            c->legal_actions = 1<<DISCARD | 1<<SWAP;
            break;

        case DISCARD:
            const bool drawn_card_wasnt_known = c->drawn_card == UNKNOWN_CARD;

            // Update discard freq table
            const enum card discarded = drawn_card_wasnt_known ? arg0 : c->drawn_card;
            ++ c->discard_ftable[discarded];
            c->last_discarded = discarded;

            // If we didn't know this card was the drawn card before this action, we now need to remove it from the
            // unseen cards table. If we already knew this was the card then it would have already been removed.
            if (drawn_card_wasnt_known)
                -- c->unseen_ftable[discarded];

            /* LEGAL ACTIONS */

            // Next player can always end the turn now by drawing
            c->legal_actions = 1<<DRAW | 1<<STICK;

            // Next player can also end the turn now by calling "cambio" if it hasn't been done already
            if (c->cambio_caller == NO_PLAYER)
                cambio_add_legal_action(c, CAMBIO);

            // In addition to the above, the following moves are also legal in the below cases:
            switch (discarded) {
                case 7:
                case 8:
                    cambio_add_legal_action(c, PEEK_OWN);
                    break;

                case 9:
                case 10:
                    cambio_add_legal_action(c, PEEK_OTHER);
                    break;

                case JACK:
                case QUEEN:
                    cambio_add_legal_action(c, BLIND_SWITCH);
                    break;

                case B_KING:
                    cambio_add_legal_action(c, B_KING_PEEK);
                    break;

                default:
                    break;
            }

            break;

        case SWAP:
            cambio_peek(c, c->turn, arg0, c->drawn_card);
            ++ c->discard_ftable[arg1];
            c->last_discarded = arg1;

            c->legal_actions = 1<<STICK | 1<<DRAW;
            if (c->cambio_caller == NO_PLAYER)
                cambio_add_legal_action(c, CAMBIO);
            break;

        case BLIND_SWITCH:
            const enum card switched_card = c->player_cards[arg0][arg1];
            c->player_cards[arg0][arg1] = c->player_cards[arg2][arg3];
            c->player_cards[arg2][arg3] = switched_card;
            cambio_remove_legal_action(c, BLIND_SWITCH);
            break;

        case PEEK_OWN:
            cambio_peek(c, c->turn, arg1, arg0);
            cambio_remove_legal_action(c, PEEK_OWN);
            break;

        case PEEK_OTHER:
            cambio_peek(c, arg2, arg1, arg0);
            cambio_remove_legal_action(c, PEEK_OTHER);
            break;

        case B_KING_PEEK:
            cambio_peek(c, arg2, arg1, arg0);
            cambio_remove_legal_action(c, PEEK_OTHER);
            cambio_add_legal_action(c, BLIND_SWITCH);
            break;

        case STICK:
            // Add the card to the discard pile
            ++ c->discard_ftable[c->last_discarded];

            // If we previously didn't know what card that was
            if (c->player_cards[arg0][arg1] == UNKNOWN_CARD) {
                // Remove it from unseen cards
                -- c->unseen_ftable[c->last_discarded];
            }

            /* Remove that card from the player's cards */

            // Null the card
            c->player_cards[arg0][arg1] = NULL_CARD;

            // Shift everything over to fill
            for (int i = arg1; i < PLAYER_MAX_CARDS - 1; ++i) {
                c->player_cards[arg0][i] = c->player_cards[arg0][i + 1];
            }

            /* If the player stuck their own card... */
            if (arg0 == c->turn)
                cambio_add_legal_action(c, GIVE_AWAY);

            cambio_remove_legal_action(c, STICK);
            break;

        case GIVE_AWAY:
            /* Add the card to the other player's cards */

            // Find the first index in their cards that is `NULL_CARD`, i.e. next available slot
            for (int i = 0; i < PLAYER_MAX_CARDS; ++i) {
                if (c->player_cards[c->turn][i] == NULL_CARD) {
                    c->player_cards[c->turn][i] = c->player_cards[arg0][arg1];
                    break;
                }
            }

            /* Remove that card from the player's cards */

            // Null the card
            c->player_cards[arg0][arg1] = NULL_CARD;

            // Shift everything over to fill
            for (int i = arg1; i < PLAYER_MAX_CARDS - 1; ++i) {
                c->player_cards[arg0][i] = c->player_cards[arg0][i + 1];
            }

            cambio_remove_legal_action(c, GIVE_AWAY);
            break;

        case CAMBIO:
            cambio_inc_turn(c);

            c->cambio_caller = c->turn;
            c->legal_actions = 1<<DRAW;
            break;

        default:
            // Unrecognized action
            break;
    }
}
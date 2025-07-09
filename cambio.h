//
// Created by Weiju Wang on 7/8/25.
//

#ifndef CAMBIO_H
#define CAMBIO_H

#include <stdbool.h>
#include <stdint.h>

#include "card.h"

/**
 * The maximum number of cards a player is assumed to be able to have.
 */
#define PLAYER_MAX_CARDS 7

/**
 * The number of cards a player has at the beginning of the game.
 */
extern const unsigned int START_NUM_CARDS;

/**
 * The state of a Cambio game with all the information known to one player.
 *
 * ### Note about the property [player_cards]
 * I coded this assuming that a player will never have more than [PLAYER_MAX_CARDS] = 7 cards. From the intial 4 cards,
 * one can only accumulate cards through penalties when they stick a card incorrectly. To get to 7 cards would mean that
 * someone incorrectly sticks 3 cards in the same game. I have never seen this happen.
 */
struct cambio {
    /**
     * The number of players in the game.
     */
    int8_t num_players;

    /**
     * The index of the player to move, with 0 being the player from whose perspective the game is being played.
     *
     * If set to -1, the game is over.
     */
    int8_t turn;

    /**
     * The index of the player who called Cambio. If no one has called it yet then this is -1.
     */
    int8_t cambio_caller;

    /**
     * A frequency table of all cards that we have not seen yet. These cards are either in the draw pile or someone's
     * cards -- i.e. all cards that are not in the discard pile that we don't know.
     *
     * "Unseen" refers to the fact that we have not seen them, but it is possible that someone else has.
     */
    uint8_t unseen_freq_table[NUM_CARD_TYPES];

    /**
     * A list of all cards belonging to players. Known cards have their values; unknown cards take `UNKNOWN_CARD`.
     *
     * The first four cards each player is dealt is numbered such that the top left is index 0, the top right is index 1,
     * the bottom left is index 2, and the bottom right is index 3. Therefore, the cards that the player sees at the
     * beginning of the game are given indices 2 and 3. If a player is penalized and gets additional cards, the new card
     * is given the next available index.
     *
     * A card that does not exist is `NULL_CARD`. You will get this if, for example, a player has 4 cards and you try to find their 5th.
     *
     * ### Example
     * To get player 2's third card, write `player_cards[2][3]`.
     */
    int8_t (*player_cards)[PLAYER_MAX_CARDS];
};

/**
 * Initializes a [cambio] struct to the beginning of the game after we see our bottom two cards. To dynamically
 * allocate and initialize a [cambio] struct, use [cambio_new].
 * @param c A pointer to the struct to initialize.
 * @param num_players The number of players in the game.
 * @param first_player The index of the player who will play first.
 * @param bottom_left The bottom-left-hand card revealed at the beginning of the game.
 * @param bottom_right The bottom-right-hand card revealed at the beginning of the game.
 * @param jokers Whether to add jokers to the deck. Two are added if this is [true].
 * @return `c` if the initialization was successful, otherwise `NULL`.
 */
struct cambio* cambio_init(struct cambio* c, unsigned int num_players, unsigned int first_player, enum card bottom_left, enum card bottom_right, bool jokers);

/**
 * Dynamically allocates and initializes a [cambio] struct to the beginning of the game after we see our bottom two cards.
 * **Warning: [cambio_new] already calls [cambio_init]. You do not need to do it yourself.**
 * @param num_players The number of players in the game.
 * @param first_player The index of the player who will play first.
 * @param bottom_left The bottom-left-hand card revealed at the beginning of the game.
 * @param bottom_right The bottom-right-hand card revealed at the beginning of the game.
 * @param jokers Whether to add jokers to the deck. Two are added if this is [true].
 * @return A pointer to the newly initialized [cambio] struct, or `NULL` if it was unsuccessful.
 */
struct cambio* cambio_new(unsigned int num_players, unsigned int first_player, enum card bottom_left, enum card bottom_right, bool jokers);

/**
 * Deep-copies a Cambio game state.
 * @return A deep copy of `c`.
 */
struct cambio* cambio_deepcopy(struct cambio* c);

/**
 * Cleans up any pointers allocated by [cambio_init]. This is for [cambio] structs that were not dynamically allocated
 * with [cambio_new].
 */
void cambio_cleanup(struct cambio* c);

/**
 * Frees the memory occupied by a [cambio] struct if it was allocated by [cambio_new]. This is for [cambio] structs that
 * were not dynamically allocated with [malloc].
 * **Warning: [cambio_free] already calls [cambio_cleanup]. You do not need to do it yourself.**
 */
void cambio_free(struct cambio* c);

/**
 *
 * @return
 */
enum card cambio_avg_unseen(struct cambio* c);

#endif //CAMBIO_H

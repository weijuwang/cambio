//
// Created by Weiju Wang on 7/8/25.
//

#ifndef CAMBIO_H
#define CAMBIO_H

#include <stdbool.h>

#include "card.h"
#include "action.h"

/**
 * The maximum number of cards a player is assumed to be able to have.
 */
#define PLAYER_MAX_CARDS 7

/**
 * A type that can store the index of a player or `NO_PLAYER`.
 */
enum player {
    NO_PLAYER = -1
};

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
    unsigned int num_players : 4;

    /**
     * The index of the player to move, with 0 being the player from whose perspective the game is being played.
     *
     * This can only be incremented by the [DRAW] and [CAMBIO] actions.
     */
    enum player turn : 4;

    /**
     * The index of the player who called Cambio. If no one has called it yet then this is -1.
     */
    enum player cambio_caller : 4;

    /**
     * The size of the draw pile. We need to keep track of these because when the draw deck runs out, the discard pile is shuffled
     * to become the new draw pile.
     */
    unsigned int draw_pile_size : 6;

    /**
     * Flags indicating which actions are legal. If there are no legal actions, the game is over.
     */
    unsigned int legal_actions : 4;

    /**
     * The last card that was drawn, if any and if known, otherwise `NULL_CARD` or `UNKNOWN_CARD`.
     *
     * This is used in the state right after someone draws a card but before that player decides what to do with the card.
     */
    enum card drawn_card : 5;

    /**
     * The card on the top of the discard pile. This is `NULL_CARD` if the discard pile is empty.
     */
    enum card last_discarded : 5;

    /**
     * A frequency table of all cards that we have not seen yet. These cards are either in the draw pile or someone's
     * cards -- i.e. all cards that are not in the discard pile that we don't know.
     *
     * "Unseen" refers to the fact that we have not seen them, but it is possible that someone else has.
     */
    card_ftable unseen_ftable;

    /**
     * The discard pile. We need to keep track of these because when the draw deck runs out, the discard pile is shuffled
     * to become the new draw pile.
     */
    card_ftable discard_ftable;

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
struct cambio* cambio_init(struct cambio* c, unsigned int num_players, enum player first_player, enum card bottom_left, enum card bottom_right, bool jokers);

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
struct cambio* cambio_new(unsigned int num_players, enum player first_player, enum card bottom_left, enum card bottom_right, bool jokers);

/**
 * Deep-copies a Cambio game state.
 * @return A deep copy of `c`.
 */
struct cambio* cambio_deepcopy(const struct cambio* c);

/**
 * Cleans up any pointers allocated by [cambio_init]. This is for [cambio] structs that were not dynamically allocated
 * with [cambio_new].
 */
void cambio_cleanup(const struct cambio*);

/**
 * Frees the memory occupied by a [cambio] struct if it was allocated by [cambio_new]. This is for [cambio] structs that
 * were not dynamically allocated with [malloc].
 * **Warning: [cambio_free] already calls [cambio_cleanup]. You do not need to do it yourself.**
 */
void cambio_free(struct cambio*);

/**
 * Increment the turn.
 */
void cambio_inc_turn(struct cambio*);

/**
 * Turn on a flag in `legal_actions` to indicate that action is legal.
 */
void cambio_add_legal_action(struct cambio*, enum action);

/**
 * Turn off a flag in `legal_actions` to indicate that actions is no longer legal.
 */
void cambio_remove_legal_action(struct cambio*, enum action);

/**
 * Remove a card from the unseen card frequency table. This is generally for internal use only.
 *
 * If the card is `UNKNOWN_CARD`, no action is taken.
 */
void cambio_remove_from_unseen(struct cambio*, enum card);

/**
 * Peek at someone's card and update relevant states. This does not check that this card was previously `UNKNOWN_CARD`
 * and will thus overwrite the value that was already there. This also doesn't check that the card is within the set of
 * possible cards, i.e. still exists in `unseen_ftable`.
 * @param p The index of the player whose card we are peeking at.
 * @param position The index of the card in our pile.
 * @param card The card that was revealed.
 */
void cambio_peek(struct cambio*, enum player p, unsigned int position, enum card card);

/**
 * Peek at one of our own cards and update relevant states. This does not check that this card was previously
 * `UNKNOWN_CARD` and will thus overwrite the value that was already there.
 * @param position The index of the card in our pile.
 * @param card The card that was revealed.
 */
void cambio_peek_own(struct cambio*, unsigned int position, enum card card);

/**
 * Remove a card
 * @param index
 */
enum card cambio_remove_player_card(struct cambio*, enum player, unsigned int index);

/**
 * Executes an action. This function will attempt to execute the action regardless of whether it is legal. The caller is
 * responsible for verifying that the action and the given arguments are legal before calling. The list of legal actions
 * (`c->legal_actions`) will be automatically updated by this method after the action is completed.
 *
 * Certain actions may require or accept arguments, which are supplied through the last five arguments. Not all
 * actions require arguments, and when arguments are required, the meaning of each argument is dependent on the action
 * taken. See the documentation for each action for information on what arguments to supply and when they are required.
 *
 * Unused arguments can take any value and it won't impact the result, but to be clear you should use `NO_PLAYER` and
 * `NULL_CARD`.
 *
 * @param c The game state on which to execute the action.
 * @param a The action to execute.
 * @param arg0 The first argument.
 * @param arg1 The second argument.
 * @param arg2 The third argument.
 * @param arg3 The fourth argument.
 */
void cambio_do_action(struct cambio* c, enum action a, int arg0, int arg1, int arg2, int arg3);

#endif //CAMBIO_H

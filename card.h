//
// Created by Weiju Wang on 7/8/25.
//

#ifndef CARD_H
#define CARD_H

#include <stdint.h>
#include <stdbool.h>

/**
 * The 15 types of meaningfully unique cards in Cambio. [UNKNOWN], or -1, is a placeholder for any card whose value we
 * don't know. 0 is used for cards that do not exist.
 *
 *
 */
enum card {
    /**
     * Stand-in for a null card.
     */
    NULL_CARD = 0,

    /* Ace through 10 do not have their own symbols because it's redundant. */

    /**
     * A jack. There are 4 of these in the deck.
     */
    JACK = 11,

    /**
     * A queen. There are 4 of these in the deck.
     */
    QUEEN = 12,

    /**
     * Black king. There are 2 of these in the deck.
     */
    B_KING,

    /**
     * Red king. There are 2 of these in the deck.
     */
    R_KING,

    /**
     * Joker. If jokers are used, there are 2 of them in the deck.
     */
    JOKER,

    /**
     * The number of card types. This is used as an upper bound in for loops when iterating through card types.
     */
    NUM_CARD_TYPES,

    /**
     * Stand-in for an unknown card.
     */
    UNKNOWN_CARD,
};

/**
 * A frequency table of card types, where `card_ftable[i]` is the number of [card]s of type `i`.
 */
typedef uint8_t card_ftable[NUM_CARD_TYPES];

/**
 * A table of information about each card type.
 */
extern const struct card_info {
    /**
     * The number of points this card is worth.
     */
    int value;

    /**
     *
     */
    char* shorthand;
} CARD_INFO[];

/**
 * Determines whether two cards are the same.
 */
bool cards_equal(enum card, enum card);

/**
 * Finds the number of cards in [ftable].
 */
unsigned int card_ftable_count(const card_ftable);

/**
 * Finds the total value of the cards in [ftable].
 */
int card_ftable_total(const card_ftable);

/**
 * Finds the average value of the cards in [ftable].
 *
 * This is more efficient than dividing the results of `card_ftable_total` and `card_ftable_count` because it only loops
 * through the table once.
 */
double card_ftable_avg(const card_ftable);

#endif //CARD_H

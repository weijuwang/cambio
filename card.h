//
// Created by Weiju Wang on 7/8/25.
//

#ifndef CARD_H
#define CARD_H

#include <stdint.h>

/**
 * The 14 types of meaningfully unique cards in Cambio. [UNKNOWN], or -1, is a placeholder for any card whose value we
 * don't know. 0 is used for cards that do not exist.
 */
enum card {
    /**
     * Stand-in for an unknown card.
     */
    UNKNOWN_CARD = -1,

    /**
     * Stand-in for a null card.
     */
    NULL_CARD = 0,

    /* Ace through 10 do not have their own symbols because it's redundant. */

    /**
     * A jack or queen. There are 8 of these in the deck.
     */
    FACE_CARD = 11,

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
    NUM_CARD_TYPES
};

/**
 * A frequency table of card types, where `card_ftable[i]` is the number of [card]s of type `i`.
 */
typedef uint8_t card_ftable[NUM_CARD_TYPES];

/**
 * A table of the values of each card type.
 */
extern const int CARD_VALUES[NUM_CARD_TYPES];

/**
 * Finds the number of points a card is worth.
 */
int card_value(enum card);

/**
 * Finds the number of cards in a frequency table.
 * @return The number of cards in [ftable].
 */
unsigned int card_ftable_count(const card_ftable);

/**
 * Finds the total value of the cards in a frequency table.
 * @return The total value of the cards in [ftable].
 */
int card_ftable_total(const card_ftable);

/**
 * Finds the average value of the cards in a frequency table.
 *
 * This is more efficient than dividing the results of `card_ftable_total` and `card_ftable_count` because it only loops
 * through the table once.
 *
 * @return The average value of the cards in [ftable].
 */
double card_ftable_avg(const card_ftable);

#endif //CARD_H

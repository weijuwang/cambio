//
// Created by Weiju Wang on 7/8/25.
//

#ifndef CARD_H
#define CARD_H

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

    NUM_CARD_TYPES
};

/**
 * A table of the values of each card type.
 */
extern const int CARD_VALUES[NUM_CARD_TYPES];

/**
 * Finds the number of points a card is worth.
 */
int card_value(enum card);

#endif //CARD_H

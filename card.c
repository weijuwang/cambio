//
// Created by Weiju Wang on 7/8/25.
//

#include "card.h"

const int CARD_VALUES[NUM_CARD_TYPES] = {
    [NULL_CARD] = 0,
    [1] = 1, [2] = 2, [3] = 3, [4] = 4, [5] = 5,
    [6] = 6, [7] = 7, [8] = 8, [9] = 9, [10] = 10,
    [FACE_CARD] = 10,
    [B_KING] = 10,
    [R_KING] = -1,
    [JOKER] = 0
};

unsigned int card_ftable_count(const card_ftable ftable) {
    int num_cards = 0;

    // Starts at 1 because null cards should never contribute to the value
    for (int i = 1; i < NUM_CARD_TYPES; ++i)
        num_cards += ftable[i];

    return num_cards;
}

int card_ftable_total(const card_ftable ftable) {
    int total = 0;

    // Starts at 1 because null cards should never contribute to the value
    for (int i = 1; i < NUM_CARD_TYPES; ++i)
        total += ftable[i] * CARD_VALUES[i];

    return total;
}

double card_ftable_avg(const card_ftable ftable) {
    int total = 0;
    int num_cards = 0;

    // Starts at 1 because null cards should never contribute to the value
    for (int i = 1; i < NUM_CARD_TYPES; ++i) {
        const int num_cards_this_type = ftable[i];
        total += num_cards_this_type * CARD_VALUES[i];
        num_cards += num_cards_this_type;
    }

    return (double) total / num_cards;
}
//
// Created by Weiju Wang on 7/8/25.
//

#include "card.h"

const struct card_info CARD_INFO[] = {
    [NULL_CARD] = {0, "-"},
    [1] = {1, "1"},
    [2] = {2, "2"},
    [3] = {3, "3"},
    [4] = {4, "4"},
    [5] = {5, "5"},
    [6] = {6, "6"},
    [7] = {7, "7"},
    [8] = {8, "8"},
    [9] = {9, "9"},
    [10] = {10, "10"},
    [JACK] = {10, "j"},
    [QUEEN] = {10, "q"},
    [B_KING] = {10, "bk"},
    [R_KING] = {-1, "rk"},
    [JOKER] = {0, "0"}
};

bool cards_equal(const enum card a, const enum card b) {
    return a == b || (a == B_KING && b == R_KING) || (a == R_KING && b == B_KING);
}

unsigned int card_ftable_count(const card_ftable ftable) {
    int num_cards = 0;

    // Starts at 1 because null cards should never contribute to the value
    for (enum card i = 1; i < NUM_CARD_TYPES; ++i)
        num_cards += ftable[i];

    return num_cards;
}

int card_ftable_total(const card_ftable ftable) {
    int total = 0;

    // Starts at 1 because null cards should never contribute to the value
    for (enum card i = 1; i < NUM_CARD_TYPES; ++i)
        total += ftable[i] * CARD_INFO[i].value;

    return total;
}

double card_ftable_avg(const card_ftable ftable) {
    int total = 0;
    int num_cards = 0;

    // Starts at 1 because null cards should never contribute to the value
    for (enum card i = 1; i < NUM_CARD_TYPES; ++i) {
        const int num_cards_this_type = ftable[i];
        total += num_cards_this_type * CARD_INFO[i].value;
        num_cards += num_cards_this_type;
    }

    return (double) total / num_cards;
}
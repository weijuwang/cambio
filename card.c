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
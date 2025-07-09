//
// Created by Weiju Wang on 7/8/25.
//

#include "card.h"

int card_value(enum card c) {
    switch (c) {
        case UNKNOWN_CARD: return 0;
        case FACE_CARD: case B_KING: return 10;
        case R_KING: return -1;
        case JOKER: return 0;
        default: return c;
    }
}
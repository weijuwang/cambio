#include "cli.h"

int main(void) {
    struct cambio* c = cambio_new(3, 0, R_KING, 10, true);
    cambio_print(c);
    cambio_do_action(c, DRAW, 3, 0, 0, 0);
    cambio_do_action(c, DISCARD, UNKNOWN_CARD, 0, 0, 0);
    cambio_print(c);
    cambio_free(c);
    return 0;
}

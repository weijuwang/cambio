#include <stdio.h>

#include "cambio.h"

int main(void) {
    struct cambio* c = cambio_new(3, 1, R_KING, 10, true);
    printf("Avg %f\n", card_ftable_avg(c->unseen_freq_table));
    printf("Count %u\n", card_ftable_count(c->unseen_freq_table));
    cambio_free(c);
    return 0;
}

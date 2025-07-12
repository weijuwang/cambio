#include <stdio.h>

#include "cambio.h"

void cambio_print(struct cambio* c) {
    printf("turn %i/%i, drawn card %i\n", c->turn, c->num_players, c->drawn_card);
    printf("cambio caller %i, ", c->cambio_caller);
    printf("draw pile size %i\n", c->draw_pile_size);
    printf("none draw disc swap swch peek stck give cmio\n");
    for (enum action i = 0; i < NUM_ACTIONS; ++i)
        printf("%c    ", c->legal_actions & 1<<i ? '!' : ' ');
    putchar('\n');
}

int main(void) {
    struct cambio* c = cambio_new(3, 1, R_KING, 10, true);
    printf("Avg %f\n", card_ftable_avg(c->unseen_ftable));
    printf("Count %u\n", card_ftable_count(c->unseen_ftable));
    //cambio_print(c);
    cambio_do_action(c, DRAW, UNKNOWN_CARD, 0, 0, 0);
    cambio_free(c);
    return 0;
}

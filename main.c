#include <stdio.h>

#include "cambio.h"

int main(void) {
    struct cambio* c = cambio_new(3, 1, B_KING, 9, true);
    struct cambio* d = cambio_deepcopy(c);
    cambio_free(c);
    cambio_free(d);
    return 0;
}

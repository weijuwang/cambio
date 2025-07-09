#include <stdio.h>

#include "cambio.h"

int main(void) {
    struct cambio* c = cambio_new(3, 1, B_KING, 9, true);
    cambio_free(c);
    return 0;
}

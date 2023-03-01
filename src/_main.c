

#include <stdio.h>
#include "test.h"

int main(void) {
    
    test2 *t0 = new_test2(10);
    test *t = new_test(t0);

    printf("i0: %d, i1: %d", t0->i,  t->i);

    return 0;
}
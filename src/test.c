#include "test.h"


test* new_test(test2 *t) {
    test *t2 = (test*) malloc(sizeof(test));
    t2->i = t->i;
    return t2;
}
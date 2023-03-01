#include "test2.h"

test2* new_test2(int i) {
    test2 *t = (test2*) malloc(sizeof(test2));
    t->i = i;
    return t;
}
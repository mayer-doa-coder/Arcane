#include <stdio.h>

int addOne(int x);

int addOne(int x) {
    int t1;

    t1=x+1;
    return t1;
}

int main() {
    int a;

    /* # ICG BEGIN */
    a=1;
    addOne(a);
    /* # ICG END */

    return 0;
}

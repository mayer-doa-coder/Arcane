#include <stdio.h>

int main() {
    int a;
    int b;
    int max;
    int i;
    int t1;
    int t2;
    int t3;
    int t4;

    /* # ICG BEGIN */
    a=5;
    b=3;
    max=0;
    i=0;
    t1=a+b;
    max=t1;
    t2=max>5;
    if (!(t2)) goto L1;
    printf("%d\n", max);
    L1: ;
    L2: ;
    t3=i<3;
    if (!(t3)) goto L3;
    printf("%d\n", i);
    t4=i+1;
    i=t4;
    goto L2;
    L3: ;
    /* # ICG END */

    return 0;
}

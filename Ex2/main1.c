#include <stdlib.h>
#include <stdio.h>

int main(){
    int i;
    int a[200];
    int b[200];
    for(i = 0;i<200;i++) {
        a[i] = i;
        b[i] = i;
    }
    b[1] = a[0]+1;
#pragma omp parallel for
    for(i = 0;i<100;i++) {
        a[2 * i + 1] = b[i] + 1;
        b[i] = a[i]
        printf("a[%d]: %d, a[%d]: %d \n",2*i+1,a[2*i+1],i,a[i]);
    }

    printf("a[101]=%d\n",a[101]);
    return 0;
}
#include <stdio.h>
int a[10][10];
int main(){
    int i, j;

#pragma omp parallel for collapse(2)
     for(i = 0;i<100;i++)
         for(j = 0;j<100;j++) {
             a[i][j] = a[i][j] + 1;
             printf("i: %d, j: %d, a: %d \n", i, j, a[i][j]);
         }

    return 0;
}

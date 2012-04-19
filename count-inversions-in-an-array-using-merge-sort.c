#include <stdio.h>

/*

The maximum number of inversions that an
n-size array can have is 

    n*(n-1) / 2

*/

int count_inversion_merge(int array[], int first, int last)
{
    int mid = (first+last)/2;
    int ai = first;
    int bi = mid+1;
    int final[last-first+1], finali=0;
    int inversion = 0, i;

    while (ai <= mid && bi <= last) {
        if (array[ai] <= array[bi]) {
                final[finali++] = array[ai++];
        } else {
                final[finali++] = array[bi++];
                inversion += mid - ai + 1;
        }
    }

    while (ai <= mid)
        final[finali++] = array[ai++];

    while (bi <= last) 
        final[finali++] = array[bi++];

    for (i=0 ; i<last-first+1 ; i++)
        array[i+first] = final[i];      

    return inversion;
}

int count_inversion(int array[], int a, int b)
{
    int x, y, z, mid;
    if (a >= b) return 0;

    mid = (a+b)/2;
    
    x = count_inversion(array, a, mid);
    y = count_inversion(array, mid+1, b);
    z = count_inversion_merge(array, a, b);

    return x+y+z;
}

int main()
{
    int a[] = {9, 8 , 7, 6, 5, 4, 3, 2, 1, 10};
    int len = 10;
    int inversion = count_inversion(a, 0, len-1);
    int i;

    for (i=0 ; i<len; i++)
            printf("%d ", a[i]);
    printf("\n");

    printf("Total inversions : %d \n", inversion);
            
    return 0;
}

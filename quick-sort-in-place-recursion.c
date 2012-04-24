#include <stdio.h>

void display(int a[], int n)
{
    int i;
    for (i=0 ; i<n ; i++)
        printf("%3d ", a[i]);
    printf("\n");
    return;
}

void swap(int a[], int index_1, int index_2)
{
    int temp = a[index_1];
    a[index_1] = a[index_2];
    a[index_2] = temp;
    
    return;
}

int partition(int a[], int first, int last)
{
    int pivot = a[first];
    int i = first + 1;
    int j;

    for (j=first+1 ; j<=last ; j++) {
        if (a[j] < pivot) {
            swap(a, i, j);
            i++;
        }
    }
    
    swap(a, first, i-1);

    return i-1;
}

void qsort(int a[], int first, int last)
{
    int mid;
    
    if (first < last) {
        mid = partition(a, first, last);
        qsort(a, first, mid-1);
        qsort(a, mid+1, last);        
    }

    return;
}


int main()
{
    // int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int a[] = {10, 1, 4, 8, 2, 5, 6, 9, 3, 7};

    int len = sizeof(a)/sizeof(a[0]);

    display(a, len);
    qsort(a, 0, len-1);
    display(a, len);

    return 0;
}


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000
#define THREAD_MAX 10

int part = 0;
int arr[SIZE];

void merge(int low, int mid, int high);

void selection_sort(int myarr, int n);

void *selection_sorting(void *param);

void merge_sort(int low, int high);

void *merge_sorting(void *param);

int main()
{
    int i, j;

    printf("\nOriginal Array: \n");

    //Duplicate checker
    for (i = 0; i < SIZE; i++)
    {
        arr[i] = rand() % 10000 + 1;

        for (j = i + 1; j < SIZE; j++)
        {
            if ((arr[i] == arr[j]) || (arr[i] == arr[j + 1]) || (arr[i] == arr[j - 1]))
            {
                //printf("\nDuplicate element: %d", arr[j]); //testing
                arr[i] = rand() % 10000 + 1;
                arr[j] = rand() % 10000 + 1;
            }
            else
                break;
        }
        printf("\t%d", arr[i]);
    }

    clock_t t1, t2;
    t1 = clock();
    pthread_t thread[THREAD_MAX];

    //if(fork() == 0)
    for (i = 0; i < THREAD_MAX; i++)
    {
        pthread_create(&thread[i], NULL, selection_sorting, (void *) NULL);
    }
    for (i = 0; i < THREAD_MAX; i++)
    {
        pthread_join(thread[i], NULL);
    }

    merge(0, (SIZE / 2 - 1) / 2, SIZE / 2 - 1);
    merge(SIZE / 2, SIZE / 2 + (SIZE - 1 - SIZE / 2) / 2, SIZE - 1);
    merge(0, (SIZE - 1) / 2, SIZE - 1);

    printf("\nSorted Array with selection sort:\n ");
    for (i = 0; i < SIZE; i++)
    {
        printf("\t%d", arr[i]);
    }

    pthread_exit(0);

    //else
    for (i = 0; i < THREAD_MAX; i++)
    {
        pthread_create(&thread[i], NULL, merge_sorting, (void *) NULL);
    }

    for (i = 0; i < THREAD_MAX; i++)
    {
        pthread_join(thread[i], NULL);
    }

    merge(0, (SIZE / 2 - 1) / 2, SIZE / 2 - 1);
    merge(SIZE / 2, SIZE / 2 + (SIZE - 1 - SIZE / 2) / 2, SIZE - 1);
    merge(0, (SIZE - 1) / 2, SIZE - 1);

    t2 = clock();

    printf("\n\nSorted Array with merge sort:\n ");
    for (i = 0; i < SIZE; i++)
    {
        printf("\t%d", arr[i]);
    }
    printf("\nTime Taken: %f", (t2 - t1) / (double) CLOCKS_PER_SEC);
    pthread_exit(0);
    return 0;
}

void selection_sort(int myarr, int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (arr[i] > arr[j])
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void *selection_sorting(void *param)
{
    int thread_part = part++;
    int low = thread_part * (SIZE / THREAD_MAX);
    int high = (thread_part + 1) * (SIZE / THREAD_MAX) - 1;
    int mid = low + (high - low) / 2;
    
    if (low < high)
    {
        selection_sort(low, mid);
        selection_sort(mid + 1, high);
        merge(low, mid, high);
    }

    pthread_exit(0);
    return 0;

}

void merge(int low, int mid, int high) //implemented from geek4geeks
{
    int *left = (int *) malloc((mid - low + 1) * sizeof(int));
    int *right = (int *) malloc((high - mid) * sizeof(int));

    int n1 = mid - low + 1,
            n2 = high - mid,
            i, j;

    for (i = 0; i < n1; i++)
        left[i] = arr[i + low];

    for (i = 0; i < n2; i++)
        right[i] = arr[i + mid + 1];

    int k = low;
    i = j = 0;

    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
            arr[k++] = left[i++];
        else
            arr[k++] = right[j++];
    }

    while (i < n1)
    {
        arr[k++] = left[i++];
    }

    while (j < n2)
    {
        arr[k++] = right[j++];
    }

    free(left);
    free(right);
}

void merge_sort(int low, int high)
{
    int mid = low + (high - low) / 2;
    if (low < high)
    {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }
}

void *merge_sorting(void *param)
{
    int thread_part = part++;

    int low = thread_part * (SIZE / THREAD_MAX);
    int high = (thread_part + 1) * (SIZE / THREAD_MAX) - 1;

    int mid = low + (high - low) / 2;
    if (low < high)
    {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }
    pthread_exit(0);
    return 0;
}


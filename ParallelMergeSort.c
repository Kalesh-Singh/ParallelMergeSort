#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 5

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

void merge(int start, int mid, int end) {
    int leftLen = mid + 1 - start;
    int rightLen = end - mid;

    int leftArr[leftLen];
    int rightArr[rightLen];

    int *lp = leftArr;
    int *lsp = &array[start];
    int *rp = rightArr;
    int *rsp = &array[mid+1];
    int *leftStart = lsp;
    while(lp != lsp + leftLen) {
        *lp++ = *leftStart++;
    }
    int *rightStart = rsp;
    while(rp != rsp + rightLen) {
        *rp++ = *rightStart++;
    }

//    std::copy(&array[start], &arr[mid] + 1, &leftArr);
//    std::copy(&array[mid+1], &arr[end] + 1, &rightArr);

}

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void* mergeSort(void* args) {
    StartEndIndexes sei = *((StartEndIndexes*) args);
    if (sei.start != sei.end) {
        pthread_t leftThread;
        pthread_t rightThread;
        int mid = (sei.start + sei.end) / 2;        // Integer division
        StartEndIndexes leftSei = {sei.start, mid};
        StartEndIndexes rightSei = {mid+1, sei.end};
        pthread_create(&leftThread, NULL, mergeSort, (void*) &leftSei);
        pthread_create(&rightThread, NULL, mergeSort, (void*) &rightSei);
        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);
    }
    return NULL;
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;

    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);

    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);

    // 3. Create a thread for merge sort.
    pthread_t mergeSortThread;
    pthread_create(&mergeSortThread, NULL, mergeSort, (void*) &sei);

    // 4. Wait for mergesort to finish.
    pthread_join(mergeSortThread, NULL);

    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);
}
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

void merge(int start, int min, int end) {

}

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void* mergeSort(void* args) {
    StartEndIndexes sei = *((StartEndIndexes*) args);
    if (sei.start != sei.end) {
        pthread_t leftThread;
        pthread_t rightThread;
        int mid = (start + end) / 2;        // Integer division
        StartEndIndexes leftSei = {start, mid};
        StartEndIndexes rightSei = {mid+1, end};
        pthread_create(&leftThread, NULL, mergeSort, (void*) &leftSei);
        pthread_create(&rightThread, NULL, mergeSort, (void*) &rightSei);
        join(leftThread, NULL);
        join(rightThread, NULL);
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
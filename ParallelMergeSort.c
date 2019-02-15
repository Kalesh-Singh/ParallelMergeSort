#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 6

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) arr[i] = rand() % 100;
}

void printArray(int arr[SIZE]) {
    for (int i = 0; i < SIZE; i++) printf("%5d", arr[i]);
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

    // Copy the sub arrays
    for (int i = start; i <= mid; i++) {
        leftArr[i - start] = array[i];
    }
    for (int i = mid + 1; i <= end; i++) {
        rightArr[i - mid - 1] = array[i];
    }

    int arrIdx = start, leftIdx = 0, rightIdx = 0;

    while (leftIdx < leftLen && rightIdx < rightLen) {
        if (leftArr[leftIdx] < rightArr[rightIdx]) {
            array[arrIdx] = leftArr[leftIdx];
            leftIdx++;
        } else {
            array[arrIdx] = rightArr[rightIdx];
            rightIdx++;
        }
        arrIdx++;
    }

    while (leftIdx < leftLen) {
        array[arrIdx] = leftArr[leftIdx];
        leftIdx++;
        arrIdx++;
    }
    while (rightIdx < rightLen) {
        array[arrIdx] = rightArr[rightIdx];
        rightIdx++;
        arrIdx++;
    }
}

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void *mergeSort(void *args) {
    StartEndIndexes sei = *((StartEndIndexes *) args);
    if (sei.start != sei.end) {
        pthread_t leftThread;
        pthread_t rightThread;
        int mid = (sei.start + sei.end) / 2;        // Integer division
        StartEndIndexes leftSei = {sei.start, mid};
        StartEndIndexes rightSei = {mid + 1, sei.end};
        pthread_create(&leftThread, NULL, mergeSort, (void *) &leftSei);
        pthread_create(&rightThread, NULL, mergeSort, (void *) &rightSei);
        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);
        merge(sei.start, mid, sei.end);
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
    pthread_create(&mergeSortThread, NULL, mergeSort, (void *) &sei);

    // 4. Wait for mergesort to finish.
    pthread_join(mergeSortThread, NULL);

    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);
}
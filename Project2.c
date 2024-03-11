#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// Global Variable
int unsortedArray[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int sortedArray[10];
int sublists[2][(10/2)+1]; // account for odd numbers of array elements if any

const int arraySize = sizeof(unsortedArray)/sizeof(unsortedArray[0]);

// Selection Sort Function
void *selectionSort(void *p_startIndex){
        int startIndex = *((int *)p_startIndex);
        int cloneUnsortedArray[arraySize];
        int sortedSublist[(10/2)+1];

        for(int i = 0; i < arraySize; i++){
            cloneUnsortedArray[i] = unsortedArray[i];
        }

        if(startIndex == 0){ // Start index is at 0
            for(int i = 0; i < arraySize/2; i++){
                int minimumIndex = i;

                for(int j = i; j < arraySize/2; j++){
                    if(cloneUnsortedArray[j] < cloneUnsortedArray[minimumIndex]){
                        minimumIndex = j;
                    }
                }

                int temp = cloneUnsortedArray[i];
                cloneUnsortedArray[i] = cloneUnsortedArray[minimumIndex];
                cloneUnsortedArray[minimumIndex] = temp;
            }

            // printf("\nFor clone sorting thread 0: ");

            // for(int i = 0; i < arraySize/2; i++){
            //     printf("%d ", cloneUnsortedArray[i]);
            // }

            for(int i = 0; i < arraySize/2; i++){
                sublists[0][i] = cloneUnsortedArray[i];
            }

            //memcpy(sublists[0], sortedSublist, ((arraySize/2)+1) * sizeof(int));
            //sublists[0] = sortedSublist;

            printf("\nFor sorting thread 0: ");

            for(int i = 0; i < arraySize/2; i++){
                printf("%d ", sublists[0][i]);
            }
        } else { // Start index is around half
            for(int i = startIndex; i < arraySize; i++){
                int minimumIndex = i;

                for(int j = i; j < arraySize; j++){
                    if(cloneUnsortedArray[j] < cloneUnsortedArray[minimumIndex]){
                        minimumIndex = j;
                    }
                }

                int temp = cloneUnsortedArray[i];
                cloneUnsortedArray[i] = cloneUnsortedArray[minimumIndex];
                cloneUnsortedArray[minimumIndex] = temp;
            }

            // printf("\nFor clone sorting thread 1: ");

            // for(int i = 0; i < arraySize/2; i++){
            //     printf("%d ", cloneUnsortedArray[i]);
            // }

            int count = 0;

            for(int i = startIndex; i < arraySize; i++){
                sublists[1][count] = cloneUnsortedArray[i];
                count++;
            }

            printf("\nFor sorting thread 1: ");

            for(int i = 0; i < arraySize/2; i++){
                printf("%d ", sublists[1][i]);
            }
        }

        pthread_exit(NULL);
}

// Merge Sublist Function
void *mergeSublist(void *args){
    int countLeft = 0;
    int countRight = 0;

    // printf("\n");

    // for(int i = 0; i < arraySize/2; i++){
    //     printf("%d ", sublists[0][i]);
    // }

    // printf("\n");

    // for(int i = 0; i < arraySize/2; i++){
    //     printf("%d ", sublists[1][i]);
    // }

    for(int i = 0; i < arraySize; i++){
        if(countLeft >= arraySize/2){
            sortedArray[i] = sublists[1][countRight];
            countRight++;

            // printf("\n");
            // printf("%d %d |", countLeft, countRight);
            // for(int i = 0; i < arraySize; i++){
            //     printf("%d ", sortedArray[i]);
            // }
            continue;
        }

        if(countRight >= arraySize/2){
            sortedArray[i] = sublists[0][countLeft];
            countLeft++;
            
            // printf("\n");
            // printf("%d %d |", countLeft, countRight);
            // for(int i = 0; i < arraySize; i++){
            //     printf("%d ", sortedArray[i]);
            // }       
            continue;
        }

        if(sublists[0][countLeft] < sublists[1][countRight]){
            sortedArray[i] = sublists[0][countLeft];
            countLeft++;
            
            // printf("\n");
            // printf("%d %d |", countLeft, countRight);
            // for(int i = 0; i < arraySize; i++){
            //     printf("%d ", sortedArray[i]);
            // }
        } else {
            sortedArray[i] = sublists[1][countRight];
            countRight++;

            // printf("\n");
            // printf("%d %d |", countLeft, countRight);
            // for(int i = 0; i < arraySize; i++){
            //     printf("%d ", sortedArray[i]);
            // }
        }
    }

    pthread_exit(NULL);
}

// Main function
int main(int argc, const char * argv[]){
    pthread_t workerThread[3];

    int startIndex = 0;
    int halfIndex = arraySize/2;

    // Create sort threads
    pthread_create(&workerThread[0], 0, selectionSort, &startIndex);
    pthread_create(&workerThread[1], 0, selectionSort, &halfIndex);

    // Wait for create sort threads
    pthread_join(workerThread[0], NULL);
    pthread_join(workerThread[1], NULL);

    // Create merge threads
    pthread_create(&workerThread[2], 0, mergeSublist, NULL);

    // Wait for merge threads
    pthread_join(workerThread[2], NULL);

    printf("\nThread complete\n");

    for(int i = 0; i < arraySize; i++){
        printf("%d ", sortedArray[i]);
    }

    return 0;
}
                                        
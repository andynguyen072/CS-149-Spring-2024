#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Global Variable
int *fibNums;

// Fib Function
void *generate_fib(void *p_numToGenerate){
        printf("Inside fib gen\n");
        int numToGenerate = *((int *)p_numToGenerate);

        // Create array size equal to specified number
        fibNums = (int *)malloc(numToGenerate * sizeof(int));

        printf("array allocated\n");
        for(int i = 0; i < numToGenerate; i++){
            if(i == 0){
                    fibNums[0] = 0;
                    continue;
            }

            if(i == 1){
                    fibNums[1] = 1;
                    continue;
            }

            fibNums[i] = fibNums[i-1] + fibNums[i-2];
        }

        printf("fib generated\n");
         pthread_exit(NULL);
}

// Main function
int main(int argc, const char * argv[]){
        printf("Enter the number of Fibonacci to generate\n");

        int numOfFib;
        pthread_t workerThread;

        scanf("%d", &numOfFib);

        pthread_create(&workerThread, 0, generate_fib, &numOfFib);
        pthread_join(workerThread, NULL);

        printf("Thread complete\n");

        for(int i = 0; i < numOfFib; i++){
            printf("%d ", fibNums[i]);
        }

        return 0;
}
                                        
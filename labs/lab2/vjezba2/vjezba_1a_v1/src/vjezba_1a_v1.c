#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3
#define NUM_PRINTS  100

static pthread_mutex_t mutex;
static int turn = NUM_THREADS; //Thread 3 (t[2]) goes first (protected by mutex)

void* print_thread(void *param)
{
    const int id = *(const int*)param;
    
    int i;
    for (i = 0; i < NUM_PRINTS; i++)
    {
        int printed = 0;
        while (!printed) // Busy waiting problem without semaphore or other way to block waiting thread
        {
            pthread_mutex_lock(&mutex); // Shared resources are: global turn variable and stdout 
            if (turn == id)
            {
                printf("%d", id);
                fflush(stdout);
                turn = (id == 1) ? NUM_THREADS : id - 1; // 3->2->1->3...
                printed = 1;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main(void)
{
    pthread_t t[NUM_THREADS];
    int ids[NUM_THREADS] = {1, 2, 3};

    pthread_mutex_init(&mutex, NULL);

    int i;
    for (i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&t[i], NULL, print_thread, &ids[i]) != 0)
        {
            fprintf(stderr, "Error creating thread %d\n", ids[i]);
            return 1;
        }
    }

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(t[i], NULL); // main thread waits here for other 3 threads to be done

    pthread_mutex_destroy(&mutex);
    printf("\n");
    return 0;
}

// OLD CODE BELOW 

/*
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* print1 (void *pParam)
{
    int i;

    for (i = 0 ; i < 1000 ; i++)
    {
        printf("1");
        fflush(stdout);
    }

    return 0;
}

void* print2 (void *pParam)
{
    int i;

    for (i = 0 ; i < 1000 ; i++)
    {
        printf("2");
        fflush(stdout);
    }

    return 0;
}

void* print3 (void *pParam)
{
    int i;

    for (i = 0 ; i < 1000 ; i++)
    {
        printf("3");
        fflush(stdout);
    }

    return 0;
}

int main (void)
{
    pthread_t hPrint1;
    pthread_t hPrint2;
    pthread_t hPrint3;

    pthread_create(&hPrint1, NULL, print1, 0);
    pthread_create(&hPrint2, NULL, print2, 0);
    pthread_create(&hPrint3, NULL, print3, 0);

    getchar();

    return 0;
}
*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_MEALS        10
#define EATING_TIME      200000 // microseconds
#define THINKING_TIME    300000 // microseconds

// One binary semaphore per fork: value 1 = fork is free, 0 = someone holds it 
static sem_t forks[NUM_PHILOSOPHERS];
/* Deadlock prevention: lets at most N-1 philosophers try to pick up forks.
   Without it, all 5 could take their left fork at once and wait forever
   for the right one (circular wait). With 4 at the table, at least one
   always gets both forks. */
static sem_t room;
// Protects stdout so messages from different threads don't get mixed up
static pthread_mutex_t printLock;
// Each thread writes only its own element, and main reads after join, so no lock is needed
static int mealsEaten[NUM_PHILOSOPHERS];

static void print_message (int id, const char *message)
{
    pthread_mutex_lock(&printLock);
    printf("Philosopher %d %s\n", id, message);
    fflush(stdout);
    pthread_mutex_unlock(&printLock);
}

void* philosopher (void *pParam)
{
    const int id = *(const int*)pParam;
    const int leftFork  = id;
    const int rightFork = (id + 1) % NUM_PHILOSOPHERS; // Wraps around for the last philosopher
    unsigned int seed = id + 1;
    
    int i;
    for (i = 0; i < NUM_MEALS; i++)
    {
        print_message(id, "is thinking.");
        // Sleeps (frees the CPU for other threads) for a random time between 150 ms and 450 ms
        usleep(THINKING_TIME / 2 + rand_r(&seed) % THINKING_TIME);  

        print_message(id, "is hungry.");
        sem_wait(&room);

        sem_wait(&forks[leftFork]);
        print_message(id, "picked up the first fork.");
        sem_wait(&forks[rightFork]);
        print_message(id, "picked up the second fork.");

        print_message(id, "is EATING.");
        // Sleeps (frees the CPU for other threads) for a random time between 100 ms and 300 ms
        usleep(EATING_TIME / 2 + rand_r(&seed) % EATING_TIME);
        mealsEaten[id]++;

        sem_post(&forks[leftFork]);
        sem_post(&forks[rightFork]);
        sem_post(&room);
    }

    print_message(id, "has finished (ate all meals).");
    return NULL;
}

int main (void)
{
    pthread_t tPhilosopher[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];
    int i;

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_init(&forks[i], 0, 1);
    }

    sem_init(&room, 0, NUM_PHILOSOPHERS - 1);

    pthread_mutex_init(&printLock, NULL);

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        ids[i] = i;
        pthread_create(&tPhilosopher[i], NULL, philosopher, &ids[i]);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(tPhilosopher[i], NULL);
    }

    printf("\nMeals per philosopher:\n");
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        printf("Philosopher %d: %d\n", i, mealsEaten[i]);
    }

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&room);
    pthread_mutex_destroy(&printLock);

    return 0;
}
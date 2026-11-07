#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_MEALS        10       // Koliko puta svaki filozof treba da jede
#define EATING_TIME      200000   // (0.2 s)
#define THINKING_TIME    300000   // us (0.3 s)

// Jedan semafor po viljuski: 1 = viljuska je slobodna, 0 = neko je drzi
static sem_t forks[NUM_PHILOSOPHERS];

// "Soba": u nju smiju istovremeno najvise 4 filozofa. Ovo sprecava deadlock
static sem_t room;

// Mutex za ispis, da se ispisi razlicitih niti ne pomijesaju
static pthread_mutex_t printLock;

// Broj obroka koje je svaki filozof pojeo (svaki filozof pise samo u svoje polje)
static int mealsEaten[NUM_PHILOSOPHERS];

static void print_message (int id, const char *message)
{
    pthread_mutex_lock(&printLock);
    printf("Filozof %d %s\n", id, message);
    fflush(stdout);
    pthread_mutex_unlock(&printLock);
}

// Tijelo niti filozofa
void* philosopher (void *pParam)
{
    int id = (int)(intptr_t)pParam;
    int leftFork  = id;                              // viljuska lijevo od filozofa 
    int rightFork = (id + 1) % NUM_PHILOSOPHERS;     // viljuska desno od filozofa 
    unsigned int seed = id + 1;                      // za razlicita nasumicna vremena 
    int i;

    for (i = 0; i < NUM_MEALS; i++)
    {
        // 1. Razmisljanje
        print_message(id, "razmislja.");
        usleep(THINKING_TIME / 2 + rand_r(&seed) % THINKING_TIME);

        // 2. Ogladnio je: trazi mjesto u sobi
        print_message(id, "je gladan.");
        sem_wait(&room);

        // 3. Uzima viljuske jednu po jednu
        sem_wait(&forks[leftFork]);
        print_message(id, "uzeo prvu viljusku.");
        sem_wait(&forks[rightFork]);
        print_message(id, "uzeo drugu viljusku.");

        // 4. Jede
        print_message(id, "JEDE.");
        usleep(EATING_TIME / 2 + rand_r(&seed) % EATING_TIME);
        mealsEaten[id]++;

        // 5. Vraca viljuske i napusta sobu
        sem_post(&forks[leftFork]);
        sem_post(&forks[rightFork]);
        sem_post(&room);
    }

    print_message(id, "je zavrsio (pojeo sve obroke).");
    return 0;
}

int main (void)
{
    pthread_t hPhilosopher[NUM_PHILOSOPHERS];
    int i;

    // Sve viljuske su na pocetku slobodne
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_init(&forks[i], 0, 1);
    }

    
    sem_init(&room, 0, NUM_PHILOSOPHERS - 1);

    pthread_mutex_init(&printLock, NULL);

    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_create(&hPhilosopher[i], NULL, philosopher, (void*)(intptr_t)i);
    }

    // Cekanje da svi filozofi zavrse
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(hPhilosopher[i], NULL);
    }

    printf("\nBroj obroka po filozofu:\n");
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        printf("Filozof %d: %d\n", i, mealsEaten[i]);
    }

    // Oslobadjanje resursa
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&room);
    pthread_mutex_destroy(&printLock);

    return 0;
}
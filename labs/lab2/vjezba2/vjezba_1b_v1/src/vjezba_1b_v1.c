#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t cs_mutex;


void* print1 (void *pParam)
{
    int i;
    for (i = 0 ; i < 100 ; i++)
    {
        printf("Zvezda");
        fflush(stdout);
        printf(" je");
        fflush(stdout);
        printf(" prvak");
        fflush(stdout);
        printf(" svijeta\n");
        fflush(stdout);
    }

    return 0;
}

void* print2 (void *lpParam)
{
    int i;
    for (i = 0 ; i < 100 ; i++)
    {
        printf("Laboratorijske");
        fflush(stdout);
        printf(" vjezbe");
        fflush(stdout);
        printf(" iz ");
        fflush(stdout);
        printf(" Operativnih");
        fflush(stdout);
        printf(" Sistema");
        fflush(stdout);
        printf(" za rad u");
        fflush(stdout);
        printf(" realnom");
        fflush(stdout);
        printf(" vremenu\n");
        fflush(stdout);
    }

    return 0;
}

void* print3 (void *lpParam)
{
    int i;
    for(i = 0 ; i < 100 ; i++)
    {
        printf("Danas");
        fflush(stdout);
        printf(" je");
        fflush(stdout);
        printf(" lijep");
        fflush(stdout);
        printf(" i");
        fflush(stdout);
        printf(" suncan");
        fflush(stdout);
        printf(" dan \n");
        fflush(stdout);
    }

    return 0;
}

int main (void)
{
    pthread_t hPrint1;
    pthread_t hPrint2;
    pthread_t hPrint3;

    pthread_mutex_init(&cs_mutex, NULL);

    pthread_create(&hPrint1, NULL, print1, 0);
    pthread_create(&hPrint2, NULL, print2, 0);
    pthread_create(&hPrint3, NULL, print3, 0);

    getchar();

    pthread_mutex_destroy(&cs_mutex);

    return 0;
}

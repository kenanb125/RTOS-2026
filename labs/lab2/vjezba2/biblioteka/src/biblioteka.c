#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


void *predrag(void *arg);
void *nenad(void *arg);

static pthread_mutex_t knjiga1=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t knjiga2=PTHREAD_MUTEX_INITIALIZER;
// ne-dinamicka inicijalizacija muteks objekta


int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, predrag, NULL);
    pthread_create(&tid2, NULL, nenad, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}

void *predrag(void *arg) {
    pthread_mutex_lock(&knjiga1);
    sleep(10);
    pthread_mutex_lock(&knjiga2);
    printf("Predrag je uzeo knjige i ode uciti!");
    pthread_mutex_unlock(&knjiga2);
    pthread_mutex_unlock(&knjiga1);
}

void *nenad(void *arg) {
    pthread_mutex_lock(&knjiga2);
    sleep(10);
    pthread_mutex_lock(&knjiga1);
    printf("Nenad je uzeo knjige... Ode uciti!");
    pthread_mutex_unlock(&knjiga1);
    pthread_mutex_unlock(&knjiga2);
}



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
int turn = 3;

void* print_thread(void *param) {
    int id = *(int*)param; 
    int i;
    for (i = 0; i < 100; i++)
    {
        int printed = 0;
        while (!printed)
        {
            pthread_mutex_lock(&mutex);
            if (turn == id)
            {
                printf("%d", id);
                fflush(stdout);
                    if (id == 3)
                        turn = 2;
                    else if (id == 2)
                        turn = 1;
                    else if (id == 1)
                        turn = 3;
                printed = 1;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main(void)
{
    pthread_t t1, t2, t3;
    
    int ids[3] = {1, 2, 3};

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t3, NULL, print_thread, &ids[2]);
    pthread_create(&t2, NULL, print_thread, &ids[1]);
    pthread_create(&t1, NULL, print_thread, &ids[0]);

    pthread_join(t3, NULL);
    pthread_join(t2, NULL);
    pthread_join(t1, NULL);

    pthread_mutex_destroy(&mutex);

    printf("\n");
    return 0;
}
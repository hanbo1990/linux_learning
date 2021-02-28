#include <mutex.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

struct td_data {
    char data[8];
    int data1;
};

static char input_buf[1000];

static sem_t sem;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *start_routine(void *data)
{
    struct td_data *thread_data;
    thread_data = (struct td_data *) data;

    printf("thread start with data %d, %s\n", thread_data->data1, thread_data->data);

    while (1) {
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
        printf("received data %s\n", input_buf);
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char **argv)
{
    pthread_t tid;
    int ret;
    char temp_buf[1000] = "\0";
    struct td_data test_data = {.data1 = 1, .data = "haha"};

    (void) sem_init(&sem, 0, 0);
    ret = pthread_create(&tid, NULL, start_routine, &test_data);
    if (0 != ret) {
        perror("pthread_create");
        return -1;
    }

    while (1) {
        fgets(temp_buf, 1000, stdin);

        pthread_mutex_lock(&mutex);
        memcpy(input_buf, temp_buf, strlen(temp_buf) + 1);
        pthread_mutex_unlock(&mutex);

        sem_post(&sem);
    }
}
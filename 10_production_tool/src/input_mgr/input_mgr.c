#include "input_mgr.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

#include "utils/custom_defines.h"
#include "utils/ringbuf.h"

RING_BUF_DEF_STATIC(event, 20, sizeof(struct input_event));

static struct input_device* input_dev = NULL;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t sem;

void* start_listen_to_input_dev(void* data)
{
    struct input_event input_event;
    struct input_device* input_dev = (struct input_device*) data;
    while (1) {
        if (0 == input_dev->get_event(&input_event)) {
            pthread_mutex_lock(&mutex);

            if (ringbuf_push(&RING_BUF_NAME(event), &input_event)) {
                LOG_WARN("Buffer full! read too slow!");
            }
            sem_post(&sem);
            pthread_mutex_unlock(&mutex);
        }
    }
}

int inputmgr_init(void)
{
    return sem_init(&sem, 0, 0);
}

int inputmgr_deinit(void)
{
    struct input_device* tmp = input_dev;
    while (tmp) {
        tmp->dev_deinit();
        tmp = tmp->next;
    }

    input_dev = NULL;

    return 0;
}

int inputmgr_resigter(struct input_device* dev)
{
    RETURN_IF_NOT_SET_WITH_LOG(dev, "invalid input");

    dev->next = input_dev;
    input_dev = dev;

    return 0;
}

int inputmgr_init_all_devices(void)
{
    pthread_t tid;
    struct input_device* tmp;

    tmp = input_dev;

    while (tmp) {
        if (0 == tmp->dev_init()) {
            LOG_INFO("Device %s inited successfully", tmp->name);
            if (0 == pthread_create(&tid, NULL, start_listen_to_input_dev, tmp)) {
                LOG_INFO("Thread for %s created successfully", tmp->name);
            } else {
                LOG_WARN("Thread for %s created failed, deiniting", tmp->name);
                (void) tmp->dev_deinit();
            }
        } else {
            LOG_INFO("Failed to init input device %s", tmp->name);
        }
        tmp = tmp->next;
    }

    return 0;
}

int inputmgr_get_event(struct input_event* input_event)
{
    sem_wait(&sem);
    pthread_mutex_lock(&mutex);

    if (0 == ringbuf_pop(&RING_BUF_NAME(event), input_event)) {
        pthread_mutex_unlock(&mutex);
        return 0;
    } else {
        LOG_ERROR("Faild to pop!");
        pthread_mutex_unlock(&mutex);
        return -1;
    }
}
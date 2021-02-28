#ifndef _INPUT_MGR
#define _INPUT_MGR

#include <sys/time.h>

#include "utils/custom_defines.h"

#define MAX_REPORT_DATA_STR_LEN 1023U
#define MAX_REPORT_DATA_MEM_LEN (MAX_REPORT_DATA_STR_LEN + 1)

enum event_type {
    EVENT_TYPE__NONE,
    EVENT_TYPE__TOUCH_SCREEN,
    EVENT_TYPE__STR_REPORT,
    EVENT_TYPE__END,
};

struct input_event {
    enum event_type type;
    struct timeval time;
    void* data;
};

struct ts_event_data {
    int x;
    int y;
    int pressure;
};

struct report_event_data {
    void (*str_used)(struct report_event_data*);
    char str[MAX_REPORT_DATA_MEM_LEN];
};

struct input_device {
    char* name;
    void (*wait_for_event)(void);
    int (*get_event)(struct input_event* input_event);
    int (*dev_init)(void);
    int (*dev_deinit)(void);
    struct input_device* next;
};

int inputmgr_resigter(struct input_device* dev);
int inputmgr_init(void);
int inputmgr_deinit(void);
void inputmgr_wait(void);
int inputmgr_get_event(struct input_event* input_event);

static inline void default_str_used(struct report_event_data* data)
{
    if (data) {
        custom_free(data);
    }
}

#define DEFAULT_REPORT_DATA(pdata)                                                                 \
    do {                                                                                           \
        pdata->str_used = default_str_used;                                                        \
    } while (0)

#endif /* _INPUT_MGR */
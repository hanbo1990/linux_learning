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

/**
 * @brief Input event reported by child and provided via API
 *
 */
struct input_event {
    enum event_type type;
    struct timeval time;
    void* data;
};

/**
 * @brief Touch screen event data
 *
 */
struct ts_event_data {
    int x;        /*< x position of the touch */
    int y;        /*< y position of the touch */
    int pressure; /*< pressure of the touch */
};

/**
 * @brief String type event data.
 *
 */
struct report_event_data {
    void (*data_used)(struct report_event_data*);
    char str[MAX_REPORT_DATA_MEM_LEN];
};

/**
 * @brief input_device defines behavior of low level child
 *
 */
struct input_device {
    char* name;
    void (*wait_for_event)(void);
    int (*get_event)(struct input_event* input_event);
    int (*dev_init)(void);
    int (*dev_deinit)(void);
    struct input_device* next;
};

static inline void default_data_used(struct report_event_data* data)
{
    if (data) {
        custom_free(data);
    }
}

/**
 * @brief Macro used when report data is dynamic memory
 *
 */
#define DEFAULT_REPORT_DATA(pdata)                                                                 \
    do {                                                                                           \
        pdata->data_used = default_data_used;                                                      \
    } while (0)

/**
 * @brief API function to init input manager
 *
 * @return 0 if success and 1 if failure
 */
int inputmgr_init(void);

/**
 * @brief API function to deinit input manager
 *
 * @return 0 if success and 1 if failure
 */
int inputmgr_deinit(void);

/**
 * @brief API function to set up all input devices
 *
 * @return 0 if success and 1 if failure
 */
int inputmgr_init_all_devices(void);

/**
 * @brief API function to get event
 *
 * @param input_event [in] Pointer to input event
 * @return 0 if success and 1 if failure
 */
int inputmgr_get_event(struct input_event* input_event);

/**
 * @brief Provide low level child modules to register themselves.
 *
 * @param dev [in] Pointer to input_device framework
 * @return 0 if success and 1 if failure
 */
int inputmgr_resigter(struct input_device* dev);

#endif /* _INPUT_MGR */
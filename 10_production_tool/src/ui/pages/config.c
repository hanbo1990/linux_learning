#include "config.h"

// clang-format off
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
// clang-format on

#include "utils/custom_defines.h"

static struct config_item configs[CONFIG_MAX_NUMBER];
static int config_count;

int config_parse_file(void)
{
    FILE* fp;
    char buf[100];
    char* p = buf;

    /* open file */
    fp = fopen(DEFAULT_CONFIG_FILE, "r");
    if (fp < 0) {
        LOG_ERROR("Failed to open config file");
        return -1;
    }

    while (fgets(buf, 100, fp)) {
        buf[99] = '\0';

        while (*p == ' ' || *p == '\t') p++;

        if (*p == '#') {
            continue;
        }

        configs[config_count].command[0] = '\0';
        configs[config_count].index = config_count;
        sscanf(p, "%s %d %s", configs[config_count].name, &configs[config_count].can_be_press,
               configs[config_count].command);
        config_count++;
    }

    return 0;
}

int config_get_item_count(void)
{
    return config_count;
}

struct config_item* config_get_item_by_index(int index)
{
    if (index < config_count) {
        return &configs[index];
    } else {
        return NULL;
    }
}

struct config_item* config_get_item_by_name(char* name)
{
    int i;

    for (i = 0; i < config_count; i++) {
        if (strcmp(name, configs[i].name) == 0) {
            return &configs[i];
        }
    }

    return NULL;
}
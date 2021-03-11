#ifndef _TS_CONFIG_H
#define _TS_CONFIG_H

#define CONFIG_MAX_NUMBER 20U
#define DEFAULT_CONFIG_FILE "/etc/ts_project/config.conf"

struct config_item{
    int index;
    int can_be_press;
    char name[100];
    char command[100];
};

int config_parse_file(void);
int config_get_item_count(void);
struct config_item* config_get_item_by_index(int index);
struct config_item* config_get_item_by_name(char* name);

#endif /* _TS_CONFIG_H */
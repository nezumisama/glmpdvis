#ifndef CMD_ARG_H
#define CMD_ARG_H

#include "common.h"

/* Errors */
#define CMD_ARG_OPTION_REQUIRES_PRAM 1

typedef void (*cmd_arg_callback)(void *data);

typedef struct {
    char *config_file_path;
    cmd_arg_callback helpfun, verfun;
    void *helpfundat, *verfundat;
} cmd_arg_config;

int parse_cmd_arg(int argc, char **argv, cmd_arg_config *conf);
void cmd_arg_default_helpfun(void *data);
void cmd_arg_default_verfun(void *data);

#endif /* CMD_ARG */

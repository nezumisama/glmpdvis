#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmd_arg.h"


int parse_cmd_arg(int argc, char **argv, cmd_arg_config *conf) {
    --argc;

    while (argc) {
        ++argv;

        if (!strcmp(*argv, "-c")) {
            if (--argc) {
                ++argv;
                conf->config_file_path = *argv;
                --argc;
            } else return CMD_ARG_OPTION_REQUIRES_PARAM;
            continue;
        }

        if (!strcmp(*argv, "-h")) {
            --argc;
            conf->helpfun(conf->helpfundat);
            continue;
        }

        if (!strcmp(*argv, "-v")) {
            --argc;
            conf->verfun(conf->verfundat);
            continue;
        }
    }

    return 0;
}

void cmd_arg_default_helpfun(void *data) {
    char * name = (char *) data;

    printf("Usage: %s [OPTION]\n"
           "\t-h - help (this text)\n"
           "\t-v - version information\n"
           "\t-c filename - use configuration file \"filename\"\n", name);

    exit(EXIT_SUCCESS);
}

void cmd_arg_default_verfun(void *data) {
    char * name = (char *) data;

    printf("%s build on %s %s\n", name, __DATE__, __TIME__);

    exit(EXIT_SUCCESS);
}

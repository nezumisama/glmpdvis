#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include "cmd_arg.h"

int main(int argc, char **argv) {
    char *name = basename(*argv);
    cmd_arg_config cac = { "<unset>", 
                           cmd_arg_default_helpfun, 
                           cmd_arg_default_verfun,
                           (void *) name, (void *) name };
    if (parse_cmd_arg(argc, argv, &cac)) { 
        printf("Parse error.\n"); 
        exit(EXIT_FAILURE); 
    }
    printf("Config file path: %s\n", cac.config_file_path);
    return EXIT_SUCCESS;
}

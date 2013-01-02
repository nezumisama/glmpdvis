#ifndef CMD_ARG_H
#define CMD_ARG_H

typedef struct {
    const char * config_file_path;
} cmd_arg_config;

void parse_cmd_arg(int argc, char ** argv, cmd_arg_config* conf); 

#endif /* CMD_ARG */

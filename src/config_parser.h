#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "common.h"

/* Errors */
#define CONFIG_PARSER_CANT_OPEN_FILE 1
/* Value tyoes */
#define CONFIG_PARSER_STRING 1
#define CONFIG_PARSER_INT 2
#define CONFIG_PARSER_FLOAT 3
#define CONFIG_PARSER_COLOR 4
#define CONFIG_PARSER_BOOL 5
#define CONFIG_PARSER_ENUM 6
/* Max. line length for the parser */
#define MAX_LINE_LENGTH 1024
/* default alpha value */
#define CONFIG_PARSER_DEFAULT_ALPHA 0xff

/* Option info struct */
typedef struct {
    const char * name;
    void * ptr;
    int type;
    const char * default_val;
    char ** enum_ptr;
    int filled;
} config_parser_option;
typedef struct {
    int n;
    config_parser_option * options;
} config_parser_options;
typedef void (*option_callback)(char * name, char * value, void* data);

int config_parser_parse_low(const char * filename, option_callback oc, void* data);
void def_option_callback(char * name, char * value, void* data);
int config_parser_parse(const char * filename, config_parser_option * options);
int config_parser_bool_parser(char* data, int* out);
int config_parser_color_parser(char* data, color* out);
void config_parser_fill_defaults(config_parser_option * options);

#endif /* CONFIG_PARSER_H */

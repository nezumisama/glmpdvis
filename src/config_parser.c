#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "config_parser.h"

int config_parser_parse_low(const char *filename, option_callback oc, void *oc_data) {
    FILE* f;
    char buff[MAX_LINE_LENGTH], opt[MAX_LINE_LENGTH], val[MAX_LINE_LENGTH];
    char *line, *opt_start, *opt_end;
    int line_len;

    f = fopen(filename, "r");
    if (!f) return CONFIG_PARSER_CANT_OPEN_FILE;

    while (!feof(f)) {
        if (!fgets(buff, MAX_LINE_LENGTH, f)) break;

        line_len = strlen(buff);
        buff[line_len - 1] = 0;
        line = &buff[0];

        while (*line && isspace(*line)) ++line;
        if (!*line) continue;
        if (*line == '#') continue;

        opt_start = line;
        while(*line && !isspace(*line)) ++line;
        if (!*line) continue;

        opt_end = line;
        while (*line && isspace(*line)) ++line;
        if (!*line) continue;

        memcpy(opt, opt_start, opt_end - opt_start);
        opt[opt_end - opt_start] = 0;
        strcpy(val, line);
        oc(opt, val, oc_data);
    }

    return 0;
}

int config_parser_parse(const char * filename, config_parser_option * options) {
    config_parser_options opts;
    int i = 0;
    opts.options = options;
    // find the length of the options list
    while (options[i].type) ++i;
    opts.n = i;
    return config_parser_parse_low(filename, config_parser_def_option_callback, &opts);
}

void config_parser_def_option_callback(char * name, char * value, void* data) {
    config_parser_options * opts = data;
    int ival;
    float fval;

    for (int i = 0; i < opts->n; ++i) {
        if (!strcmp(name, opts->options[i].name)) {
            switch (opts->options[i].type) {
                case CONFIG_PARSER_STRING:
                    *((char **) opts->options[i].ptr) = malloc(strlen(value) + 1);
                    strcpy(*((char **) opts->options[i].ptr), value);
                    opts->options[i].filled = 1;
                    break;
                case CONFIG_PARSER_INT:
                    if (sscanf(value, "%d", &ival)) {
                        *((int*) opts->options[i].ptr) = ival;
                        opts->options[i].filled = 1;
                    }
                    break;
                case CONFIG_PARSER_FLOAT:
                    if (sscanf(value, "%f", &fval)) {
                        *((float*) opts->options[i].ptr) = fval;
                        opts->options[i].filled = 1;
                    }
                    break;
                case CONFIG_PARSER_BOOL:
                    if (!config_parser_bool_parser(value, ((int*) opts->options[i].ptr)))
                        opts->options[i].filled = 1;
                    break;
                case CONFIG_PARSER_COLOR:
                    if (!config_parser_color_parser(value, ((color*) opts->options[i].ptr)))
                        opts->options[i].filled = 1;
                    break;
            }
            break;
        }
    }
}

int config_parser_bool_parser(char* data, int* out) {
    static char * true_strings[] = { "true", "yes", 0 };
    static char * false_strings[] = { "false", "no", 0 };
    char **s = &true_strings[0];

    while (*s) {
        if (!strcmp(data, *s)) {
            *out = 1;
            return 0;
        }
        ++s;
    }

    s = &false_strings[0];
    while (*s) {
        if (!strcmp(data, *s)) {
            *out = 0;
            return 0;
        }
        ++s;
    }
    return -1;
}

int config_parser_color_parser(char* data, color* out) {
    unsigned int r, g, b, a;

    if (sscanf(data, "%2x%2x%2x", &r, &g, &b) < 3) return -1;
    if (sscanf(data + 6, "%2x", &a) != 1) a = CONFIG_PARSER_DEFAULT_ALPHA;

    out->r = r*ONE_PER_255; out->g = g*ONE_PER_255;
    out->b = b*ONE_PER_255; out->a = a*ONE_PER_255;

    return 0;
}

void config_parser_fill_defaults(config_parser_option * options) {
    config_parser_option *o = &options[0];
    config_parser_options opts;
    int i = 0;

    opts.options = options;
    // find the length of the options list
    while (options[i].type) ++i;
    opts.n = i;

    while (o->type != 0) {
        if (!o->filled) {
            config_parser_def_option_callback((char*) o->name,
                                              (char*) o->default_val, &opts);
        }
        ++o;
    }
}

int config_parser_load_config(const char* filename,
                              config_parser_option *options) {
    int ret = config_parser_parse(filename, options);
    if (ret) return ret;
    config_parser_fill_defaults(options);
    return 0;
}

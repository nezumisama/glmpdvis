#include <stdio.h>
#include "config_parser.h"
#include "config.h"

int main(int argc, char ** argv) {
    config_parser_parse("config", option_list);
    config_parser_fill_defaults(option_list);
    printf("***\n");
    printf("fifo_path = %s\n", config.fifo_path);
    printf("samples = %d\n", config.samples);
    printf("length = %d\n", config.length);
    printf("print fps = %s\n", config.print_fps?"yes":"no");
    printf("distance = %f\n", config.distance);
    printf("bg_color = %.3f, %.3f, %.3f, %.3f\n", config.bg_color.r, config.bg_color.g, config.bg_color.b, config.bg_color.a);
    return 0;
}

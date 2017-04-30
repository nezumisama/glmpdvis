#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "config_parser.h"
#include "common.h"

struct {
    char * fifo_path;
    int samples;
    int length;
    int print_fps;
    float distance;
    color bg_color;
} config;

config_parser_option option_list[] = {
    // name, address of data in config struct, type, default value (as string),
    // enum list address, was filled (should be 0)
    {"fifo_path", &config.fifo_path, CONFIG_PARSER_STRING, "/tmp/mpd.fifo", 0,
     0},
    {"samples", &config.samples, CONFIG_PARSER_INT, "512", 0, 0},
    {"length", &config.length, CONFIG_PARSER_INT, "80", 0, 0},
    {"print_fps", &config.print_fps, CONFIG_PARSER_BOOL, "no", 0, 0},
    {"distance", &config.distance, CONFIG_PARSER_FLOAT, "100.0", 0, 0},
    {"bg_color", &config.bg_color, CONFIG_PARSER_COLOR, "000000ff", 0, 0},
    {0, 0, 0, 0, 0, 0} // Sentinel
};

#endif /* CONFIG_H */

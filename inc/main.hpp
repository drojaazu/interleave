#include <getopt.h>
#include <stdio.h>
#include <exception>
#include <fstream>
#include <iostream>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

u8 process_args(int argc, char **argv);
void print_help();
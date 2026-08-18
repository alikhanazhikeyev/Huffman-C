#pragma once

#include <stdio.h>      // Input and Output
#include <stdint.h>     // For uint64_t etc.

typedef struct {
    FILE *output;
    uint8_t buffer;     // Current Bit in a Byte (0-7)
    uint8_t count;      // Current Byte in a File
} BitWriter;

typedef struct {
    FILE *input;
    uint8_t buffer;
    uint8_t count;
} BitReader;

void bit_writer_init(BitWriter *bw, FILE *output);
void bit_writer_write_code(BitWriter *bw, uint64_t code, uint8_t len);
void bit_writer_flush(BitWriter *bw);

void bit_reader_init(BitReader *br, FILE *output);
int bit_reader_read_bit(BitReader *br);
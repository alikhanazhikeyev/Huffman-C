/*---------------------------------------------------------
    Abstraction Wrappers for BitWriter and BitReader

    An abstraction wrapper in C is a function, structure,
    or file that hides complex, low-level details. It 
    exposes a simple, clean set of functions so other 
    parts of a program do not need to know how things 
    work underneath.
----------------------------------------------------------*/

#include "bit_io.h"

/*----- Initializing an Abstraction Wrapper -----*/
void bit_writer_init(BitWriter *bw, FILE *output) 
{
    bw->output = output;
    bw->buffer = 0;
    bw->count = 0;
}

/*----- Writing a single code to a file -----*/
void bit_writer_write_code(BitWriter *bw, uint64_t code, uint8_t len)
{
    // Исправлено условие: i >= 0
    for (int i = len - 1; i >= 0; i--) {
        /*----- Calculate bit to write -----*/
        uint8_t bit = (code >> i) & 1;

        /*----- Write to buffer -----*/
        bw->buffer = (bw->buffer << 1) | bit;

        /*----- Update count -----*/
        bw->count++;

        /*----- Check if buffer is full (8 bits) -----*/
        if (bw->count == 8) {
            fputc(bw->buffer, bw->output);
            bw->count = 0;
            bw->buffer = 0;
        }
    }
}

/*----- In case if last bits were not written into a file -----*/
void bit_writer_flush(BitWriter *bw)
{
    if (bw->count > 0) {
        bw->buffer = bw->buffer << (8 - bw->count);
        fputc(bw->buffer, bw->output);
        bw->buffer = 0;
        bw->count = 0;
    }
}

void bit_reader_init(BitReader *br, FILE *input)
{
    br->input = input;
    br->buffer = 0;
    br->count = 0;
}

int bit_reader_read_bit(BitReader *br)
{
    if (br->count == 0) {
        int ch = fgetc(br->input);
        if (ch == EOF) {
            return -1;
        }
        br->buffer = (uint8_t)ch;
        br->count = 8;
    }
    br->count--;
    int bit = (br->buffer >> br->count) & 1;
    return bit;
}
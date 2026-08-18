#include <string.h>     // For strcmp()
#include <stdlib.h>     // malloc, free
#include "heap.h"
#include "huffman.h"


int main(int argc, char *argv[])
{
    /*----- Validation -----*/
    if (argc < 4) {
        fprintf(stderr, "Usage: %s -mode <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    /*----- Opening files and validating -----*/
    FILE *input_file = fopen(argv[2], "rb");  // "rb": read binary
    FILE *output_file = fopen(argv[3], "wb");
    if (input_file == NULL || output_file == NULL) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }

    /*----- Compression (-c flag) -----*/
    if (strcmp(argv[1], "-c") == 0) {
        char *dot = strrchr(argv[3], '.');
        if (dot == NULL || strcmp(dot + 1, "huff") != 0) {
            fprintf(stderr, "Warning: Output file should end in .huff\n");
        }
        huffman_compress(input_file, output_file);
    }
    /*----- Decompress (-d flag) -----*/
    else if (strcmp(argv[1], "-d") == 0) {
        char *dot = strrchr(argv[2], '.');
        if (dot == NULL || strcmp(dot + 1, "huff") != 0) {
            fprintf(stderr, "Error: Input file must be a .huff file.\n");
            fclose(input_file);
            fclose(output_file);
            return 1;
        }
        huffman_decompress(input_file, output_file);
    } 
    /*----- Nonvalid mode -----*/
    else {
        fprintf(stderr, "Invalid mode input.\n Valid modes:\n1) \"-c\" for compression\n2) \"-d\" for decompression.\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
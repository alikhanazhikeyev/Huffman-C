#pragma once

#include <stdio.h>      // Input and Output
#include <stddef.h>     // For size_t and NULL
#include <stdint.h>     // For uint64_t etc.
#include <inttypes.h>   // For PRIu64 format specifier (for uint64_t)
#include <stdlib.h>     // malloc, free
#include "heap.h"
#include "bit_io.h"

typedef struct {
    uint64_t code;
    uint8_t len;    // Depth or level of a Node in the tree
} HuffmanCode;

Node* huffman_tree(FILE *file);
void huffman_destruct(Node *root);

void huffman_codes(Node *root, uint64_t code, uint8_t len, HuffmanCode huffman_table[]);

void huffman_compress(FILE *input_file, FILE *output_file);
void huffman_decompress(FILE *input_file, FILE *output_file);

Node* huffman_reconstruct(uint32_t freq[256]);
int huffman_decode(BitReader *br, Node *root);
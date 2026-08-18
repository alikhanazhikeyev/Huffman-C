#pragma once

#include <stdbool.h>    // For bool, true, and false
#include <stddef.h>     // For size_t and NULL
#include <stdlib.h>     // malloc, free
#include <stdio.h>      // Input and Output

/*----- Fields of a Node -----*/
typedef struct Node {
    unsigned char symbol;
    uint32_t frequency;
    struct Node *left;
    struct Node *right;
} Node;

/*----- Fields of a Heap -----*/
typedef struct MinHeap {
    Node *heap[257];  // We have only 256 charachters in ASCII
    size_t capacity;
    size_t size;
} MinHeap;


Node* node_construct(unsigned char symbol);
void node_destruct(Node* self);

MinHeap* heap_construct();
void heap_destroy(MinHeap *self);

void heap_insert(MinHeap *self, Node* elem);
void up_heap(MinHeap *self);

Node* heap_min(const MinHeap *self);
Node* heap_rm_min(MinHeap *self);
void down_heap(MinHeap *self);

bool heap_is_empty(const MinHeap *self);
void swap(MinHeap *self, size_t i, size_t j);
size_t heap_find_min(MinHeap *self, size_t idx);
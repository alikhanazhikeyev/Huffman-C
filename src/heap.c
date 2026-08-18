/* --------------------------------------------------------------
    This is a code for a Priority Queue – Min Heap in C language
    desidned for Huffman's tree. 
-------------------------------------------------------------- */

#include "heap.h"

/*----- Node Construction -----*/
Node* node_construct(unsigned char symbol)
{
    Node *node = malloc(sizeof(*node));
    node->symbol = symbol;
    node->frequency = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*----- Node Destruction -----*/
void node_destruct(Node* self)
{
    if (self == NULL) return;
    node_destruct(self->left);
    node_destruct(self->right);
    free(self);
}

/*----- Heap Construction -----*/
MinHeap* heap_construct() 
{
    MinHeap *heap = malloc(sizeof(*heap));
    if (heap == NULL) return NULL;
    heap->capacity = 256;
    heap->size = 0;
    return heap;
}

/*---- Heap Destruction -----*/
void heap_destroy(MinHeap *self)
{
    if (self == NULL) return;
    free(self);
}

/*----- Inserting an Element -----*/
void heap_insert(MinHeap *self, Node* elem)
{
    /* ! We do not check the size since it is guaranreed not to exceed 256 ! */
    self->heap[++self->size] = elem;
    up_heap(self);
}

void up_heap(MinHeap *self)
{
    size_t idx = self->size;
    size_t parent;

    while (idx > 1) {
        parent = idx / 2;
        if (self->heap[parent]->frequency <= self->heap[idx]->frequency) {
            break;
        }
        swap(self, idx, parent);
        idx = parent;
    }
}

/*----- Exctracting a Minimum Element -----*/
Node* heap_min(const MinHeap *self)
{
    if (self == NULL || heap_is_empty(self)) return NULL;
    return self->heap[1];
}

/*----- Removing a Minimum Element -----*/
Node* heap_rm_min(MinHeap *self) 
{
    if (self == NULL || heap_is_empty(self)) return NULL;
    Node* tmp = heap_min(self);
    self->heap[1] = self->heap[self->size--];
    down_heap(self);
    return(tmp);
}

void down_heap(MinHeap *self)
{
    size_t idx = 1;
    size_t min_child;

    while (2 * idx <= self->size) {
        min_child = heap_find_min(self, idx);
        if (self->heap[idx]->frequency <= self->heap[min_child]->frequency) {
            break;
        }
        swap(self, idx, min_child);
        idx = min_child;
    }
}


/*----- Helper Functions -----*/
bool heap_is_empty(const MinHeap *self)
{
    if (self == NULL) return 1;
    return self->size == 0;
}

size_t heap_find_min(MinHeap *self, size_t idx)
{
    size_t left = 2 * idx;
    size_t right = 2 * idx + 1;

    // "Right child doesn't exist" case
    if (right > self->size) return left;

    if (self->heap[left]->frequency < self->heap[right]->frequency) return left;
    else return right;
}

void swap(MinHeap *self, size_t i, size_t j)
{
    Node *tmp = self->heap[i];
    self->heap[i] = self->heap[j];
    self->heap[j] = tmp;
}


/*-----------------------------------------------------------------------

    To test:

    clang -DTEST_HEAP heap.c -o test_heap
    ./test_heap


    To debug:

    clang -DTEST_HEAP -g heap.c -o test_heap
    lldb ./test_heap

    b main              – breakpoint
    run                 – executes everything before the breakpoint
    n                   – next line
    p <variable_name>   – prints a variable or structure in the memory

-----------------------------------------------------------------------*/
#ifdef TEST_HEAP
int main() 
{
    MinHeap *heap = heap_construct();

    Node *n1 = node_construct('a'); n1->frequency = 100;
    Node *n2 = node_construct('b'); n2->frequency = 50;
    Node *n3 = node_construct('c'); n3->frequency = 10;

    heap_insert(heap, n1);
    assert(heap->size == 1);

    heap_insert(heap, n2);
    assert(heap->size == 2);

    heap_insert(heap, n3);
    assert(heap->size == 3);

    assert(heap_min(heap)->frequency == 10);

    Node *m1 = heap_rm_min(heap);
    assert(m1->frequency == 10);
    assert(heap->size == 2);

    Node *m2 = heap_rm_min(heap);
    assert(m2->frequency == 50);
    assert(heap->size == 1);

    Node *m3 = heap_rm_min(heap);
    assert(m3->frequency == 100);
    assert(heap_is_empty(heap) == true);

    node_destruct(n1);
    node_destruct(n2);
    node_destruct(n3);
    heap_destroy(heap);

    printf("All right\n");

    return 0;
}
#endif
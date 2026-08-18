#include "heap.h"
#include "huffman.h"
#include "bit_io.h"


/*----- Building a Huffman's Tree -----*/
Node* huffman_tree(FILE *file) 
{
    /* ---------------------------------------------------------------------------------------------
        Counting the frequency of each character, where the position in the array is the ASCII code
        for a character, e.g. 'a' is in freq[97]
    --------------------------------------------------------------------------------------------- */
    uint32_t freq[256] = {0};
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        freq[(unsigned char)ch]++;
    }

    /*----- Inserting the nodes with counted frequencies into a Min Heap -----*/
    MinHeap *heap = heap_construct();
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            Node *n = node_construct((unsigned char)i);
            n->frequency = freq[i];

            heap_insert(heap, n);
        }
    }

    /*----- Huffman's Tree Construction -----*/
    while (heap->size > 1) {
        Node *left = heap_rm_min(heap);
        Node *right = heap_rm_min(heap);

        Node *parent = node_construct(0);
        parent->left = left;
        parent->right = right;
        parent->frequency = left->frequency + right->frequency;

        heap_insert(heap, parent);
    }

    Node *root = heap_rm_min(heap);
    heap_destroy(heap);
    return root; 
}

/*----- Huffman's Tree Destruction -----*/
void huffman_destruct(Node *root) 
{
    if (root->left) {
        huffman_destruct(root->left);
    }
    if (root->right) {
        huffman_destruct(root->right);
    }
    free(root);
}

/*----- Generate codes -----*/
void huffman_codes(Node *root, uint64_t code, uint8_t len, HuffmanCode table[])
{
    if (!root) return;

    // Leaf Node
    if (root->left == NULL && root->right == NULL) {
        table[root->symbol].code = code;
        if (len == 0) {
            table[root->symbol].len = 1;
        } else {
            table[root->symbol].len = len;
        }
        return;
    }

    // Have Left
    if (root->left) {
        huffman_codes(root->left, code << 1, len + 1, table);
    }

    // Have Right
    if (root->right) {
        huffman_codes(root->right, code << 1 | 1, len + 1, table);
    }
 }

/*-------------------------------------------------------
    File Compression using Bite Wrapper Functions
-------------------------------------------------------*/
void huffman_compress(FILE *input_file, FILE *output_file)
{
    /*----- Write metadata in a header of the file -----*/
    uint32_t freq[256] = {0};
    int i;
    while ((i = fgetc(input_file)) != EOF) {
        freq[(unsigned char)i]++;
    }
    fwrite(freq, sizeof(uint32_t), 256, output_file);    // Exactly 1024 Bytes
    rewind(input_file);

    /*----- Build a tree -----*/
    Node *root = huffman_tree(input_file);
    if (!root) return;

    /*----- Generate codes -----*/
    HuffmanCode table[256] = {0};
    huffman_codes(root, 0, 0, table);
    rewind(input_file);

    /*----- Writing the codes using BitWriter -----*/
    BitWriter bw;
    bit_writer_init(&bw, output_file);
    int j;
    while ((j = fgetc(input_file)) != EOF) {
        bit_writer_write_code(&bw, table[(unsigned char)j].code, table[(unsigned char)j].len);
    }
    bit_writer_flush(&bw);

    huffman_destruct(root);
}


/*-------------------------------------------------------
    File Decompression using Bite Wrapper Functions
-------------------------------------------------------*/
void huffman_decompress(FILE *input_file, FILE *output_file)
{
    /*----- Read the header and reconstruct the frequency table -----*/
    uint32_t freq[256] = {0};
    size_t count = fread(freq, sizeof(uint32_t), 256, input_file);
    if (count != 256) {
        fprintf(stderr, "Corrupted file header\n");
        fclose(input_file);
        fclose(output_file);
        return;
    }

    /*----- Reconstruct a tree -----*/
    Node* root = huffman_reconstruct(freq);

    BitReader br;
    bit_reader_init(&br, input_file);

    uint32_t total_chars = root->frequency;
    if (total_chars == 0) {
        huffman_destruct(root);
        return;
    }

    for(uint32_t i = 0; i < total_chars; i++) {
        int symbol = huffman_decode(&br, root);
        if (symbol == -1) break;

        fputc((unsigned char)symbol, output_file);
    }
    
    huffman_destruct(root);
}

Node* huffman_reconstruct(uint32_t freq[256]) 
{
    MinHeap *heap = heap_construct();
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            Node *n = node_construct((unsigned char)i);
            n->frequency = freq[i];

            heap_insert(heap, n);
        }
    }

    while (heap->size > 1) {
        Node *left = heap_rm_min(heap);
        Node *right = heap_rm_min(heap);

        Node *parent = node_construct(0);
        parent->left = left;
        parent->right = right;
        parent->frequency = left->frequency + right->frequency;

        heap_insert(heap, parent);
    }

    Node* root = heap_rm_min(heap);
    heap_destroy(heap);
    return root; 
}

int huffman_decode(BitReader *br, Node *root)
{
    Node *curr = root;
    while (curr->left != NULL || curr->right != NULL) {
        int bit = bit_reader_read_bit(br);
        
        if (bit == -1) {
            return -1;  // <-----------
        }

        if (bit == 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return curr->symbol;    // <-----------
}


/*-----------------------------------------------------------------------

    To test:

    clang -DTEST_HUFFMAN huffman.c -o test_heap
    ./test_heap


    To debug:

    clang -DTEST_HEAP -g huffman.c -o test_heap
    lldb ./test_heap

    b main              – breakpoint
    run                 – executes everything before the breakpoint
    n                   – next line
    p <variable_name>   – prints a variable or structure in the memory
    
-----------------------------------------------------------------------*/
#ifdef TEST_HUFFMAN
int main(int argc, char *argv[])
{
    // Testing code generating function for debugging
    // FILE *input = fopen("test_file.txt", "rb");
    // Node *root = huffman_tree(input);
    // HuffmanCode table[256] = {0};
    // huffman_codes(root, 0, 0, table);


    /*----- Testing validity of compression-decompression cycle -----*/
    FILE *input = fopen("test_input.txt", "rb");
    FILE *output = fopen("test_output.huff", "wb");

    if (!input || !output) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }

    huffman_compress(input, output);

    fclose(input);
    fclose(output);

    output = fopen("test_output.huff", "rb");
    FILE *result = fopen("test_result.txt", "wb");

    if (!output || !result) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }

    huffman_decompress(output, result);

    fclose(output);
    fclose(result);

    input = fopen("test_input.txt", "rb");
    result = fopen("test_result.txt", "rb");

    if (!input || !result) {
        fprintf(stderr, "Error opening files\n");
        return 1;
    }

    int ch1;
    int ch2;
    while (1) {
        ch1 = fgetc(input);
        ch2 = fgetc(result);

        if (ch1 != ch2) {
            fprintf(stderr, "The initial file does not align with the result\n");
            return 1;
        }

        if ((ch1 == EOF) && (ch2 == EOF)) {
            break;
        }
    }

    fclose(input);
    fclose(result);

    printf("Successful program execution and logic.");
    return 0;
}
#endif
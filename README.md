# Huffman Algoritm Implementation in C without External Libraries

## Project Overview
This project is a lossless data compression utility implemented in pure C based on the classical Huffman Coding algorithm. Built with zero external dependencies and a decoupled modular architecture, the tool provides a fast command-line interface for compressing and decompressing arbitrary files.

### Key Architecture & Highlights:
- Custom Min-Heap: Implements an array-based binary priority queue to construct optimal prefix-free prefix trees with O(NlogK) complexity.
- Bit-Level Stream I/O: Features custom BitWriter and BitReader abstractions that buffer, pack, and unpack variable-length bit sequences into 8-bit aligned disk writes.
- Universal Binary Compatibility: Evaluates full 8-bit frequency distributions (0–255), enabling seamless processing of ASCII, UTF-8 multilingual text, documents, and compiled binaries.

### Compression Characteristics
- Plain Text: Works best on uncompressed text (.txt, .c, etc).
- Pre-Compressed Media (.pdf, .png, .mp3): These formats are already compressed internally, leaving almost no repetitive patterns to shrink. While the algorithm won't reduce their size (~0% gain), it still decodes and restores them without any data loss.

## Compression Efficiency
> **Note:** I used Gemini to write a shell-script for calculating algorithm efficiency for different file formats in test-files folder.

### The results are:

| File | Original Size | Compressed Size | Compression Ratio | Integrity |
| :--- | :--- | :--- | :--- | :--- |
| `book.pdf` | 2,155,228 B (~2.06 MB) | 2,145,504 B (~2.05 MB) | +0.45% | OK |
| `huffman.c` | 7,585 B (~7.4 KB) | 5,492 B (~5.4 KB) | +27.59% | OK |
| `large_text.txt` | 1,140,001 B (~1.09 MB) | 576,025 B (~562.5 KB) | **+49.47%** | OK |
| `small_text.txt` | 1,153 B (~1.1 KB) | 1,709 B (~1.7 KB) | -48.22% | OK |
| `tree.png` | 1,281,945 B (~1.22 MB) | 1,282,769 B (~1.22 MB) | -0.06% | OK |
| `vision_udiennx.mp3` | 4,391,750 B (~4.19 MB) | 4,380,105 B (~4.18 MB) | +0.27% | OK |

> **Note:** `small_text.txt` shows a negative compression ratio due to the fixed 1024-byte frequency table header. Pre-compressed formats (`.pdf`, `.png`, `.mp3`) yield non-sigificant results because their internal redundancy is already minimized, yet all files maintain 100% integrity upon decompression.

## Execution Speed & Scalability

Throughput: **~14.2 MB/s**.

### Benchmark Data
> **Note:** I used Gemini to generate `speed_test.py` script

| Input Size | Compression Time | Throughput | 
| :--- | :--- | :--- | 
| **1 KB** | 0.0032 s | 0.31 MB/s | 
| **10 KB** | 0.0035 s | 2.79 MB/s | 
| **100 KB** | 0.0098 s | 9.96 MB/s | 
| **1 MB** | 0.0729 s | 13.72 MB/s | 
| **10 MB** | 0.7453 s | 13.42 MB/s | 
| **50 MB** | 3.4567 s | 14.46 MB/s | 
| **100 MB** | 6.9855 s | 14.32 MB/s | 
| **500 MB** | 35.2426 s | 14.19 MB/s | 
| **1 GB** | 72.2708 s | 14.17 MB/s | 

---

### Complexity Breakdown

* **Tree Construction $\mathcal{O}(K \log K)$:** Fixed alphabet size ($K \le 256$) bounds tree generation to a constant sub-millisecond cost ($\approx 2048$ operations max).
* **Data Processing $\mathcal{O}(N)$:** Two-pass sequential streaming (frequency profiling followed by bit-level encoding) yields a strictly linear runtime profile across arbitrarily large payloads.

## How to compile & run
**Compile:**
``` sh
make
```

**Compression Syntax:**
``` bash
./huffman -c <input_file> <compressed_output_file> 
```

**Decompression Syntax:**
``` bash
./huffman -d <compressed_file> <decompressed_output_file>
```
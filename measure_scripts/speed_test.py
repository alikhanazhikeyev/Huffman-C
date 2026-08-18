import os
import time
import subprocess
import shutil

# This python script was entirely writen by Gemini Generative-AI Chat-Bot

C_EXEC = "./Huffman-C/huffman"
TMP_DIR = "speed_test_tmp"

SIZES = [
    ("1 KB", 1024),
    ("10 KB", 10 * 1024),
    ("100 KB", 100 * 1024),
    ("1 MB", 1024 * 1024),
    ("10 MB", 10 * 1024 * 1024),
    ("50 MB", 50 * 1024 * 1024),
    ("100 MB", 100 * 1024 * 1024),
    ("500 MB", 500 * 1024 * 1024),
    ("1 GB", 1024 * 1024 * 1024),
]

def generate_file(filepath, target_bytes):
    phrase = (b"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
              b"Integer nec odio. Praesent libero. Sed cursus ante dapibus diam.\n")
    chunk = phrase * ((64 * 1024) // len(phrase) + 1)
    
    written = 0
    with open(filepath, "wb") as f:
        while written < target_bytes:
            to_write = min(len(chunk), target_bytes - written)
            f.write(chunk[:to_write])
            written += to_write

def main():
    if not os.path.exists(C_EXEC):
        subprocess.run(["make", "-C", "Huffman-C"], check=True)

    os.makedirs(TMP_DIR, exist_ok=True)

    print("=" * 30)
    print(f"{'Input Size':<12} | {'Time (s)':<12}")
    print("=" * 30)

    for label, byte_size in SIZES:
        src_file = os.path.join(TMP_DIR, "input.txt")
        comp_file = os.path.join(TMP_DIR, "output.huff")

        generate_file(src_file, byte_size)

        start = time.perf_counter()
        res = subprocess.run([C_EXEC, "-c", src_file, comp_file], 
                             stdout=subprocess.DEVNULL, 
                             stderr=subprocess.DEVNULL)
        elapsed = time.perf_counter() - start

        time_str = f"{elapsed:8.4f} s" if res.returncode == 0 else "ERROR"
        print(f"{label:<12} | {time_str:<12}")

        if os.path.exists(src_file):
            os.remove(src_file)

    print("=" * 30)
    shutil.rmtree(TMP_DIR)

if __name__ == "__main__":
    main()
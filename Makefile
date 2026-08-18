CC = clang
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude

SRCDIR = src
OBJDIR = obj
INCDIR = include

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

TARGET = huffman

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

debug: CFLAGS += -fsanitize=address,undefined -g
debug: clean $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET) benchmark_tmp

.PHONY: all debug clean
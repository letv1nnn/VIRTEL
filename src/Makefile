CC = gcc
CFLAGS = -Wall
TARGET = virtel
SOURCES = main.c chunk.c debug.c malloc.c vm.c scanner.c compiler.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run:
	$(TARGET)

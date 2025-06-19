#include "include/common.h"
#include "include/chunk.h"
#include "include/debug.h"
#include "include/vm.h"

#include <string.h>
#include <stdlib.h>

void test_chunk_vm();

static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

static char *readFile(const char *path) {
    FILE *file = fopen(path, "rb");
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char *buffer = (char *)virtel_malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(1);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Couldn't read file \"%s\".\n", path);
        exit(1);
    }
    buffer[bytesRead] = '\0';
    fclose(file);
    return buffer;
}

static void runFile(const char *path) {
    char *source = readFile(path);
    InterpretResult result = interpret(source);
    virtel_free(source);
    if (result == COM_ERR || result == RUN_ERR) exit(1);
}

int main(int argc, char **argv) {
    Chunk *chunk = constructChunk();
    constructVM(chunk);

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(1);
    }

    deconstructVM();
    return 0;
}


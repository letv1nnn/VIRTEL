#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


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
    if (file == NULL) {
        fprintf(stderr, "Could not open the file.\n");
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file.\n");
        exit(1);
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}


static void runFile(const char *path) {
    char *source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) exit(1);
    if (result == INTERPRET_RUNTIME_ERROR) exit(1);
}


int main(int argc, char **argv) {
    createVM();

    if (argc == 1) {
        repl(); // single line scrpting
    } else if (argc == 2) {
        runFile(argv[1]); // file to read
    } else {
        fprintf(stderr, "Usage: VIRTEL [path]\n");
        exit(1);
    }

    freeVM();
    return 0;
}

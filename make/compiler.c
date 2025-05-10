#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "virtualMachine.h"

#define TOKEN_TYPE_COUNT 40

typedef struct {
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
}Parser;

Parser parser;
Chunk* compilingChunk;

static void errorAt(Token *token, const char *message) {
    if (parser.panicMode) return;
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR) {
        // nothing yet
    }
    else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char* message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

bool compile(const char* source, Chunk* chunk) {
    initLexer(source);
    parser.hadError = false;
    parser.panicMode = false;

    for (;;) {
        Token token = ScanToken();

        if (token.type == TOKEN_NUMBER) {
            double value = strtod(token.start, NULL);

            int constantIndex = writeConstant(chunk, value, parser.previous.line);
            writeChunk(chunk, OP_CONSTANT, token.line);
            writeChunk(chunk, (uint8_t)constantIndex, token.line);
        }
        else if (token.type = TOKEN_STRING) {
            printf("%.*s\n", token.length, token.start);
            break;
        }
        else if (token.type == TOKEN_EOF) {
            writeChunk(chunk, OP_RETURN, token.line);
            break;
        }
        else {
            errorAt(&token, "Unexpected token.");
            return false;
        }
    }

    return !parser.hadError;
}

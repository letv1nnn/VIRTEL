#include "include/compiler.h"
#include "include/common.h"

#include <stdlib.h>

typedef struct {
    Token current, previous;
    bool hadError, panicMode;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY,
} Precedence;

typedef void (*ParseFn) ();

typedef struct {
    ParseFn prefix, infix;
    Precedence precedence;
} ParseRule;

Parser parser;
Chunk *compilingChunk;

static void error(const char *message) {
    if (parser.panicMode) {
        return;
    }
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", parser.previous.line);
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void advance() {
    parser.previous = parser.current;
    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;

        error(parser.current.start);
    }
}

static void consume(TokenType type, const char *message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    error(message);
}

static void addOpcode(uint8_t opcode) {
    writeByte(compilingChunk, opcode, parser.previous.line);
}

static void addTwoOpcodes(uint8_t first, uint8_t second) {
    addOpcode(first);
    addOpcode(second);
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    uint8_t index = writeConstants(compilingChunk->constants, value);
    if (index > 255) {
        error("To many constants in one chunk");
        return;
    }
    addTwoOpcodes(OP_CONSTANT, index);
}

static void grouping();
static void unary();
static void expression();
static void binary();
static void parsePrecedence(Precedence precedence);
static ParseRule *getRule(TokenType type);

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
  [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
  [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
  [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
  [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
  [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
  [TOKEN_MINUS] = {unary, binary, PREC_TERM},
  [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
  [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
  [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
  [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
  [TOKEN_BANG] = {NULL, NULL, PREC_NONE},
  [TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
  [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
  [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
  [TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
  [TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
  [TOKEN_LESS] = {NULL, NULL, PREC_NONE},
  [TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
  [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
  [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
  [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
  [TOKEN_AND] = {NULL, NULL, PREC_NONE},
  [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
  [TOKEN_FALSE] = {NULL, NULL, PREC_NONE},
  [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
  [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
  [TOKEN_IF] = {NULL, NULL, PREC_NONE},
  [TOKEN_NOTH] = {NULL, NULL, PREC_NONE},
  [TOKEN_OR] = {NULL, NULL, PREC_NONE},
  [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
  [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
  [TOKEN_TRUE] = {NULL, NULL, PREC_NONE},
  [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
  [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
  [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
  [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};


static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary() {
    TokenType opType = parser.previous.type;
    parsePrecedence(PREC_UNARY);
    switch (opType) {
        case TOKEN_MINUS:
            addOpcode(OP_NEG);
            break;
        default:
            return;
    }
}

static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_PLUS:
            addOpcode(OP_ADD);
            break;
        case TOKEN_MINUS:
            addOpcode(OP_SUB);
            break;
        case TOKEN_STAR:
            addOpcode(OP_MUL);
            break;
        case TOKEN_SLASH:
            addOpcode(OP_DIV);
            break;
        default:
            break;
    }
}

static void parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression.");
        return;
    }
    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

static ParseRule *getRule(TokenType type) {
    return &rules[type];
}

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}


bool compile(const char *source, Chunk *chunk) {
    constructScanner(source);
    parser.hadError = false;
    parser.panicMode = false;
    compilingChunk = chunk;

    advance(); // primes the pump on the scanner
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");

    addOpcode(OP_RETURN);
    return !parser.hadError;
}

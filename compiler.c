#include "compiler.h"

typedef struct {
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
} Parser;

typedef void(*ParseFn)();

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
  PREC_PRIMARY
} Precedence;

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

static void grouping();
static void number();
static void unary();
static void binary();
static void expression();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);


ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

Parser parser;
Chunk *compilingChunk;


static Chunk *compilingChunk() {
    return compilingChunk;
}

static void errorAt(Token *token, const char *message) {
    if (parser.panicMode == true) return;
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF)
        fprintf(stderr, " at the end.");
    else if (token->type == TOKEN_ERROR) {

    }
    else
        fprintf(stderr, " at '%.*s'", token->length, token->start);

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

// errors handling
static void error(const char *message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;
        errorAtCurrent(parser.current.start);
    }
}


static void consume(TokenType type, const char *message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}
// Emitting bytecode


static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t first, uint8_t second) {
    emitByte(first);
    emitByte(second);
}

static uint8_t makeConstant(Value value) {
    int constant = writeConstant(currentChunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk!\n.);
        return 0;
    }
    return (uint8_t)constant;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, writeCosntant(value));
}

static void endCompiler() {
    emitByte(OP_RETURN);

    if (!parser.hadError) {
        disassembleChunk(currentChunk(), "code");
    }
}

static void group() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

static void expression();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

static void unary() {
    TokenType operatorType = parser.previous.type;

    parsePrecedence(PREC_UNARY);

    switch(operatorType) {
        case TOKEN_MINUS: emitByte(OP_NEG); break;
        default: return;
    }
}

static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_PLUS:          emitByte(OP_ADD); break;
        case TOKEN_MINUS:         emitByte(OP_SUB); break;
        case TOKEN_STAR:          emitByte(OP_MUL); break;
        case TOKEN_SLASH:         emitByte(OP_DIV); break;
        default: return; // Unreachable.
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
    initScanner(source);
    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

#if 0 // scanner testing purposes
    int line = -1;
    for (;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);
        if (token.type == TOKEN_EOF) break;
    }
#endif

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.\n");
    endCompiler();
    return !parser.hadError;
}


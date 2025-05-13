#include "scanner.h"
#include <ctype.h>

#include <string.h>

typedef struct {
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void initScanner(const char *src) {
    scanner.start = src;
    scanner.current = src;
    scanner.line = 1;
}


static Token createToken(TokenType type) {
    Token token;
    token.line = scanner.line;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    return token;
}

static Token createErrorToken(const char *message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.line = scanner.line;
    token.start = message;
    token.line = scanner.line;
    token.length = (int)strlen(message);
    return token;
}

static char peekNext() {
    if (*scanner.current == '\0') return '\0';
    return scanner.current[1];
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static bool match(char expected) {
    if (*scanner.current == '\0') return false;
    if (*scanner.current != expected) return false;
    return true;
}

static void skip() {
    for (;;) {
        char c = *scanner.current;
        switch (c) {
            case '\r': case ' ': case '\t':
                advance(); break;
            case '\n':
                scanner.line++;
                advance();break;
            case '/':
                if (peekNext() == '/') {
                    while (c != '\n' && c != '\0') advance();
                } else {
                    return;
                }
            default: return;
        }
    }
}


static Token string() {
    while (*scanner.current != '\0' && *scanner.current != '"') {
        if (*scanner.current == '\n') {
            scanner.line++;
        }
        advance();
    }
    if (*scanner.current == '\0') {
        return createErrorToken("Unterminated string.\n");
    }
    advance();
    return createToken(TOKEN_STRING);
}

static Token number() {
    while (isdigit(*scanner.current)) {
        advance();
    }
    if (*scanner.current == '.' && isdigit(peekNext())) {
        advance();
        while (isdigit(*scanner.current)) advance();
    }

    return createToken(TOKEN_NUMBER);
}

static TokenType keyword(int start, int length, const char *rest, TokenType type) {
    if ((scanner.current - scanner.start == start + length) && memcmp(scanner.start + start, rest, length) == 0)
        return type;
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
    switch(scanner.start[0]) {
        case 'a': return keyword(1, 2, "nd", TOKEN_AND);
        case 'c': return keyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return keyword(1, 3, "lse", TOKEN_ELSE);
        case 'i': return keyword(1, 1, "f", TOKEN_IF);
        case 'n': return keyword(1, 2, "il", TOKEN_NIL);
        case 'o': return keyword(1, 1, "r", TOKEN_OR);
        case 'p': return keyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return keyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return keyword(1, 4, "uper", TOKEN_SUPER);
        case 'v': return keyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return keyword(1, 4, "hile", TOKEN_WHILE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return keyword(2, 3, "lse", TOKEN_FALSE);
                    case 'u': return keyword(2, 1, "n", TOKEN_FUN);
                    case 'o': return keyword(2, 1, "r", TOKEN_FOR);
                }
            }
            break;
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return keyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return keyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
    }

    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isalpha(*scanner.current) || isdigit(*scanner.current)) advance();
    return createToken(identifierType());
}

Token scanToken() {
    skip();
    scanner.start = scanner.current;
    if (*scanner.current == '\0') return createToken(TOKEN_EOF);

    char c = advance();

    if (c >= '0' && c <= '9') {
        return number();
    }
    if (isalpha(c) || c == '_') {
        return identifier();
    }

    switch (c) {
        case '(': return createToken(TOKEN_LEFT_PAREN);
        case ')': return createToken(TOKEN_RIGHT_PAREN);
        case '{': return createToken(TOKEN_LEFT_BRACE);
        case '}': return createToken(TOKEN_RIGHT_BRACE);
        case ';': return createToken(TOKEN_SEMICOLON);
        case ',': return createToken(TOKEN_COMMA);
        case '.': return createToken(TOKEN_DOT);
        case '-': return createToken(TOKEN_MINUS);
        case '+': return createToken(TOKEN_PLUS);
        case '/': return createToken(TOKEN_SLASH);
        case '*': return createToken(TOKEN_STAR);
        case '!': return createToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return createToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return createToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return createToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }

    return createErrorToken("Unexpected token.");
}


#include "include/scanner.h"
#include "include/common.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void constructScanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static Token makeToken(TokenType type){
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static char advance() {
    if (*scanner.current == '\0') return '\0';
    scanner.current++;
    return scanner.current[-1];
}

static bool match_next_char(char expected) {
    if (*scanner.current == '\0') return false;
    return *(scanner.current + 1) == expected;
}

static void skipWhitespace() {
    for (;;) {
        char c = *scanner.current;
        switch (c) {
            case '\r':
            case '\t':
            case  ' ':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (*(scanner.current + 1) == '/') {
                    while (*scanner.current != '\n' && *scanner.current != '\0') {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static Token number() {
    while (isdigit(*scanner.current)) {
        advance();
    }
    if (*scanner.current == '.' && isdigit(scanner.current[1])) {
        advance();
        while (isdigit(*scanner.current)) {
            advance();
        }
    }
    return makeToken(TOKEN_NUMBER);
}

static Token string() {
    //advance();
    while (*scanner.current != '"' && *scanner.current != '\0') {
        if (*scanner.current == '\n') {
            scanner.line++;
        }
        advance();
    }
    if (*scanner.current == '\0') {
        return makeToken(TOKEN_ERROR);
    }
    advance();
    return makeToken(TOKEN_STRING);
}

static TokenType checkKeyword(int start, int length, char const *rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
      memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 3, "oth", TOKEN_NOTH);
        case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
        case 't': return checkKeyword(1, 3, "rue", TOKEN_TRUE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (isalpha(*scanner.current)
            || isdigit(*scanner.current)
            || *scanner.current == '_') {
        advance();
    }
    return makeToken(identifierType());
}


Token scanToken() {
    skipWhitespace();
    scanner.start = scanner.current;

    if (*scanner.current == '\0') return makeToken(TOKEN_EOF);

    char c = advance();
    if (isalpha(c)) return identifier();
    if (isdigit(c)) return number();

    switch (c) {
        case '+': return makeToken(TOKEN_PLUS);
        case '-': return makeToken(TOKEN_MINUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_LEFT_BRACE);
        case '.': return makeToken(TOKEN_DOT);
        case ',': return makeToken(TOKEN_COMMA);
        case ';': return makeToken(TOKEN_SEMICOLON);
        // cases with two possible tokens
        case '<':
            if (match_next_char('=')) {
                advance();
                return makeToken(TOKEN_LESS_EQUAL);
            } else {
                return makeToken(TOKEN_LESS);
            }
        case '>':
            if (match_next_char('=')) {
                advance();
                return makeToken(TOKEN_GREATER_EQUAL);
            } else {
                return makeToken(TOKEN_GREATER);
            }
        case '=':
            if (match_next_char('=')) {
                advance();
                return makeToken(TOKEN_EQUAL_EQUAL);
            } else {
                return makeToken(TOKEN_EQUAL);
            }
        case '!':
            if (match_next_char('=')) {
                advance();
                return makeToken(TOKEN_BANG_EQUAL);
            } else {
                return makeToken(TOKEN_BANG);
            }
        case '"':
            return string();
        default:
            return makeToken(TOKEN_ERROR);
    }
}


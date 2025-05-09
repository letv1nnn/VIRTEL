#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "lexer.h"

typedef struct {
	const char* start;
	const char* current;
	int line;
} Lexer;


Lexer lexer;

static bool reachEnd() {
	return *lexer.current == '\0';
}

static bool compareNext(char expected) {
	if (reachEnd()) return '\0';
	return expected == *(lexer.current + 1);
}

void initLexer(const char* source) {
	lexer.start = source;
	lexer.current = source;
	lexer.line = 1;
}

static void skipWhitespaces() {
	for (;;) {
		char c = *lexer.current;
		switch (c) {
			case ' ':
			case '\r':
			case '\t':
				lexer.current++;
				break;
			case '\n':
				lexer.current++;
				lexer.line++;
				break;
			case '/':
				if (*(lexer.current + 1) == '/') {
					while (*lexer.current != '\n' && !reachEnd()) lexer.current++;
				}
				else {
					return;
				}
			default: return;
		}
	}
}

static Token createToken(TokenType type) {
	Token token;
	token.type = type;
	token.line = lexer.line;
	token.start = lexer.start;
	token.length = (int)(lexer.current - lexer.start);
	return token;
}

static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.line = lexer.line;
	token.length = (int)strlen(message);
	return token;
}

static Token number() {
	while (isdigit(*lexer.current)) lexer.current++;
	if (*lexer.current == '.' && isdigit(*(lexer.current + 1))) {
		lexer.current++;
		while (isdigit(*lexer.current)) lexer.current++;
	}
	return createToken(TOKEN_NUMBER);
}

static Token string() {
	while (*lexer.current != '"' && !reachEnd()) {
		if (*lexer.current == '\n') lexer.line++;
		lexer.current++;
	}
	if (reachEnd()) return errorToken("Unterminated string!");
	lexer.current++;
	return createToken(TOKEN_STRING);
} 

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
	if (lexer.current - lexer.start >= start + length && memcmp(lexer.start + start, rest, length) == 0) {
		return type;
	}
	return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
	switch (lexer.start[0]) {
		case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
		case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
		case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
		case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
		case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
		case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
		case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
		case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
		case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
		case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
		case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
		case 'f':
			if (lexer.current - lexer.start > 1) {
				switch (lexer.start[1]) {
					case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
					case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
				}
			}
			break;
		case 't':
			if (lexer.current - lexer.start > 1) {
				switch (lexer.start[1]) {
					case 'h':
						return checkKeyword(2, 2, "is", TOKEN_THIS);
					case 'r':
						return checkKeyword(2, 2, "ue", TOKEN_TRUE);

				}
				break;
			}
	}
	return TOKEN_IDENTIFIER;
}

static Token identifiers() {
	while (isalpha(*lexer.current) ||
		*lexer.current == '_' ||
		isdigit(*lexer.current)) {
		lexer.current++;
	}
	return createToken(identifierType());
}

static Token ScanToken() {
	skipWhitespaces();
	lexer.start = lexer.current;

	if (reachEnd()) return createToken(TOKEN_EOF);

	char c = *lexer.current++;
	// check identifier, keyword;
	if (isalpha(c) || c == '_') return identifiers();
	if (isdigit(c)) return number();

	switch (c) {
		case '+': return createToken(TOKEN_PLUS);
		case '-': return createToken(TOKEN_MINUS);
		case '*': return createToken(TOKEN_STAR);
		case '/': return createToken(TOKEN_SLASH);
		case ';': return createToken(TOKEN_SEMICOLON);
		case '.': return createToken(TOKEN_DOT);
		case ',': return createToken(TOKEN_COMMA);
		case '(': return createToken(TOKEN_LEFT_PAREN);
		case ')': return createToken(TOKEN_RIGHT_PAREN);
		case '{': return createToken(TOKEN_LEFT_BRACE);
		case '}': return createToken(TOKEN_RIGHT_BRACE);
		case '"': return string();

		case '<':
			if (compareNext('=')) {
				lexer.current++;
				return createToken(TOKEN_LESS_EQUAL);
			}
			return createToken(TOKEN_LESS);
		case '>':
			if (compareNext('=')) {
				lexer.current++;
				return createToken(TOKEN_GREATER_EQUAL);
			}
			return createToken(TOKEN_GREATER);
		case '=':
			if (compareNext('=')) {
				lexer.current++;
				return createToken(TOKEN_EQUAL_EQUAL);
			}
			return createToken(TOKEN_EQUAL);
		case '!':
			if (compareNext('=')) {
				lexer.current++;
				return createToken(TOKEN_BANG_EQUAL);
			}
			return createToken(TOKEN_BANG);
	}

	return errorToken("Unexpected character!");
}
#include "common.h"
#include "scanner.h"

#include <stdio.h>
#include <string.h>

Scanner scanner;

void init_scanner(const char* source) {
    scanner.line = 1;
    scanner.start = source;
    scanner.current = source;   
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_at_end() {
    return *scanner.current == '\0';
}

static char peek(int d) {
    return *(scanner.current + d);
}

static char advance() {
    return *(scanner.current++);
}

static bool match(char expected) {
    if (is_at_end()) return false;
    if (*scanner.current != expected) return false;

    scanner.current++;
    return true;
}

static void skip_white_space() {
    for (;;) {
        switch (peek(0)) {
            case ' ' :
            case '\t':
            case '\r':
                advance();
                break;

            case '\n':
                scanner.line++;
                advance();
                break;

            case '/':
                if (peek(1) == '/') {
                    while (peek(0) != '\n' && !is_at_end()) advance();
                } else {
                    return;
                }
                break;

            default:
                return;
        }
    }
}

static TokenType check_keyword(int start, int length, const char* rest, TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static Token make_token(TokenType type) {
    Token token;

    token.type = type;
    token.line = scanner.line;
    token.start = scanner.start;
    token.lenght = (int) (scanner.current - scanner.start);
    
    return token;
}

static Token error_token(const char* message) {
    Token token;

    token.type = TOKEN_ERROR;
    token.line = scanner.line;
    token.start = message;
    token.lenght = (int) strlen(message);

    return token;
}

static Token string(char quote) {
    while (peek(0) != quote && !is_at_end()) {
        if (peek(0) == '\n') scanner.line++;
        advance();
    }

    if (is_at_end()) return error_token("unterminated string");

    advance();
    return make_token(TOKEN_STRING);
}

static Token number() {
    while (is_digit(peek(0))) advance();

    if (peek(0) == '.' && is_digit(peek(1))) {
        advance();
        while (is_digit(peek(0))) advance();
    }

    return make_token(TOKEN_NUMBER);
}

static TokenType identifier_type() {
    switch (scanner.start[0]) {
        case 'a': return check_keyword(1, 2, "nd", TOKEN_AND);
        case 'c': return check_keyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return check_keyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return check_keyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return check_keyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return check_keyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i': return check_keyword(1, 1, "f", TOKEN_IF);
        case 'n': return check_keyword(1, 2, "il", TOKEN_NIL);
        case 'o': return check_keyword(1, 1, "r", TOKEN_OR);
        case 'p': return check_keyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return check_keyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return check_keyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'h': return check_keyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return check_keyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v': return check_keyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return check_keyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while (is_alpha(peek(0)) || is_digit(peek(0))) advance();

    return make_token(identifier_type());
}

Token scan_token() {
    skip_white_space();
    scanner.start = scanner.current;

    if (is_at_end()) {
        return make_token(TOKEN_EOF);
    }

    char c = advance();

    if (is_digit(c)) return number();
    if (is_alpha(c)) return identifier();

    switch (c) {
        case '(': return make_token(TOKEN_LEFT_PAREN);
        case ')': return make_token(TOKEN_RIGHT_PAREN);
        case '{': return make_token(TOKEN_LEFT_BRACE);
        case '}': return make_token(TOKEN_RIGHT_BRACE);
        case ',': return make_token(TOKEN_COMMA);
        case '.': return make_token(TOKEN_DOT);
        case ';': return make_token(TOKEN_SEMI_COLON);
        case '+': return make_token(TOKEN_PLUS);
        case '-': return make_token(TOKEN_MINUS);
        case '/': return make_token(TOKEN_SLASH);
        case '*': return make_token(TOKEN_STAR);

        case '<': return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '!': return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '>': return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

        case '"' : return string('"');
        case '\'': return string('\'');
    }

    return error_token("unexpected character");
}
#include "common.h"
#include "scanner.h"
#include "compiler.h"

#include <stdio.h>

void compile(const char* source) {
    int line = -1;
    init_scanner(source);
    
    for (;;) {
        Token token = scan_token();

        if (token.line == line) {
            printf("   | ");
        } else {
            line = token.line;
            printf("%4d ", token.line);
        }

        printf("%2d '%.*s'\n", token.type, token.lenght, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}
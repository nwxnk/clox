#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "couldn't open file %s\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "not enough memory to load %s\n", path);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "couldn't read file %s\n", path);
        exit(74);
    }
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

void repl() {}

void run_file(const char* path) {
    char* source = read_file(path);
    InterpreterResult result = interpret(source);
    free(source);

    switch (result) {
        case INTERPRETER_OK: break;
        case INTERPRETER_COMPILE_ERROR: exit(65);
        case INTERPRETER_RUNTIME_ERROR: exit(70);
    }
}

int main(int argc, char** argv) {
    init_vm();

    switch (argc) {
        case 1: repl(); break;
        case 2: run_file(argv[1]); break;
        default:
            fprintf(stderr, "usage: %s [path]\n", argv[0]);
            exit(64);
    }

    free_vm();
    return 0;
}
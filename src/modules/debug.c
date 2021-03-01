#include "debug.h"
#include <stdio.h>

void disassemble_chunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    
    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

static int simple_instruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassemble_instruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);

    switch (chunk->code[offset]) {
        case OP_RETURN:
            return simple_instruction("OP_RETURN", offset);
        default:
            printf("unknown opcode: %s\n", chunk->code[offset]);
            return offset + 1;
    }
}
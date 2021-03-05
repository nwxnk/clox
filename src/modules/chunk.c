#include "chunk.h"
#include "values.h"
#include "memory.h"
#include <stdlib.h>

void init_chunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->lines = NULL;
    chunk->code = NULL;
    init_value_array(&chunk->constants);
}

void write_chunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->count + 1 > chunk->capacity) {
        int capacity = GROW_CAPACITY(chunk->capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, chunk->capacity, capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, chunk->capacity, capacity);
        chunk->capacity = capacity;
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void free_chunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    free_value_array(&chunk->constants);
    init_chunk(chunk);
}

int add_constant(Chunk* chunk, Value value) {
    write_value_array(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void write_constant(Chunk* chunk, Value value, int line) {
    int index = add_constant(chunk, value);

    if (index <= 0xff) {
        write_chunk(chunk, OP_CONSTANT, line);
        write_chunk(chunk, index, line);
    } else {
        write_chunk(chunk, OP_CONSTANT_LONG, line);
        write_chunk(chunk, index >> 16 & 0xff, line);
        write_chunk(chunk, index >> 8  & 0xff, line);
        write_chunk(chunk, index >> 0  & 0xff, line);
    }
}
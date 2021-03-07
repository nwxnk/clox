#ifndef clox_chunk_h
#define clox_chunk_h

#include "value.h"
#include "common.h"

typedef enum {
    OP_RETURN,
    OP_NEGATE,
    OP_CONSTANT,
    OP_CONSTANT_LONG,
} OPCODE;

typedef struct {
    int count;
    int capacity;
    int* lines;
    uint8_t* code;
    ValueArray constants;
} Chunk;

void init_chunk (Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte, int line);
void free_chunk (Chunk* chunk);

int add_constant(Chunk* chunk, Value value);
void write_constant(Chunk* chunk, Value value, int line);

#endif
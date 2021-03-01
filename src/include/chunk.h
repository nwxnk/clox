#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"

typedef enum {
    OP_RETURN
} OPCODE;

typedef struct {
    int count;
    int capacity;
    int* lines;
    uint8_t* code;
} Chunk;

void init_chunk (Chunk* chunk);
void write_chunk(Chunk* chunk, uint8_t byte, int line);
void free_chunk (Chunk* chunk);

#endif
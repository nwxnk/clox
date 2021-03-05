#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "common.h"

typedef enum {
    INTERPRETER_OK,
    INTERPRETER_COMPILE_ERROR,
    INTERPRETER_RUNTIME_ERROR,
} InterpreterResult;

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
} VM;

void init_vm();
void free_vm();

InterpreterResult interpret(Chunk* chunk);

#endif
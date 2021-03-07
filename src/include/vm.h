#ifndef clox_vm_h
#define clox_vm_h

#include "value.h"
#include "chunk.h"
#include "common.h"

#define STACK_MAX 256

typedef enum {
    INTERPRETER_OK,
    INTERPRETER_COMPILE_ERROR,
    INTERPRETER_RUNTIME_ERROR,
} InterpreterResult;

typedef struct {
    Value* sp;
    uint8_t* ip;
    Chunk* chunk;
    Value stack[STACK_MAX];
} VM;

void init_vm();
void free_vm();

void push_stack(Value value);
Value pop_stack();

InterpreterResult interpret(Chunk* chunk);

#endif
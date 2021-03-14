#include "vm.h"
#include "chunk.h"
#include "value.h"
#include "debug.h"
#include "common.h"
#include "compiler.h"

#include <stdio.h>
#include <stdarg.h>

VM vm;

static void reset_stack() {
    vm.sp = vm.stack;
}

void init_vm() {
    reset_stack();
}

void free_vm() {

}

void push_stack(Value value) {
    *(vm.sp++) = value;
}

Value pop_stack() {
    return *(--vm.sp);
}

static Value peek(int distance) {
    return vm.sp[-1 - distance];
}

static void runtime_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script\n", line);

    reset_stack();
}

static InterpreterResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_LONG_CONSTANT() (vm.chunk->constants.values[ \
    READ_BYTE() << 16 | \
    READ_BYTE() <<  8 | \
    READ_BYTE() <<  0])

#define BINARY_OP(value_type, op) \
    do {\
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtime_error("operands must be numbers"); \
            return INTERPRETER_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop_stack()); \
        double a = AS_NUMBER(pop_stack()); \
        push_stack(value_type(a op b)); \
    } while (false)

    uint8_t instruction;
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        if (vm.sp != vm.stack) {
            printf("        [");
            for (Value* slot = vm.stack; slot < vm.sp; slot++) {
                print_value(*slot);
                printf(", ");
            }
            printf("\b\b]\n");
        }
        disassemble_instruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
#endif
        switch(instruction = READ_BYTE()) {
            case OP_NIL:   push_stack(NIL_VAL); break;
            case OP_TRUE:  push_stack(BOOL_VAL(true)); break;
            case OP_FALSE: push_stack(BOOL_VAL(false)); break;

            case OP_ADD:      BINARY_OP(NUMBER_VAL, +); break;
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;

            case OP_NEGATE: {
                if (!IS_NUMBER(peek(0))) {
                    runtime_error("operand must be a number");
                    return INTERPRETER_RUNTIME_ERROR;
                }
                push_stack(NUMBER_VAL(-AS_NUMBER(pop_stack())));
                break;
            }
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push_stack(constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_LONG_CONSTANT();
                push_stack(constant);
                break;                
            }
            case OP_RETURN: {
                return INTERPRETER_OK;
            }
        }
    }

#undef BINARY_OP

#undef READ_LONG_CONSTANT
#undef READ_CONSTANT
#undef READ_BYTE
}

InterpreterResult interpret(const char* source) {
    Chunk chunk;
    init_chunk(&chunk);

    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return INTERPRETER_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpreterResult result = run();

    free_chunk(&chunk);
    return result;
}
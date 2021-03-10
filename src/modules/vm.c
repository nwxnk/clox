#include "vm.h"
#include "chunk.h"
#include "value.h"
#include "debug.h"
#include "common.h"
#include "compiler.h"

#include <stdio.h>

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
    *vm.sp = value;
    vm.sp++;
}

Value pop_stack() {
    vm.sp--;
    return *vm.sp;
}

static InterpreterResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_LONG_CONSTANT() (vm.chunk->constants.values[ \
    READ_BYTE() << 16 | \
    READ_BYTE() <<  8 | \
    READ_BYTE() <<  0])

#define BINARY_OP(op) \
    do {\
        Value b = pop_stack(); \
        Value a = pop_stack(); \
        push_stack(a op b); \
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
            case OP_ADD: BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_DIVIDE: BINARY_OP(/); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_RETURN: {
                return INTERPRETER_OK;
            }
            case OP_NEGATE: {
                push_stack(-pop_stack());
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
        }
    }

#undef BINARY_OP

#undef READ_LONG_CONSTANT
#undef READ_CONSTANT
#undef READ_BYTE
}

InterpreterResult interpret(const char* source) {
    compile(source);
    return INTERPRETER_OK;
}
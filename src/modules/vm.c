#include "vm.h"
#include "debug.h"
#include "common.h"

#include <stdio.h>

VM vm;

void init_vm() {

}

void free_vm() {

}

static InterpreterResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_LONG_CONSTANT() (vm.chunk->constants.values[ \
    READ_BYTE() << 16 | \
    READ_BYTE() <<  8 | \
    READ_BYTE() <<  0])

    uint8_t instruction;
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        disassemble_instruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
#endif
        switch(instruction = READ_BYTE()) {
            case OP_RETURN:
                return INTERPRETER_OK;
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                print_value(constant);
                printf("\n");
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = READ_LONG_CONSTANT();
                print_value(constant);
                printf("\n");
                break;                
            }
        }
    }

#undef READ_LONG_CONSTANT
#undef READ_CONSTANT
#undef READ_BYTE
}

InterpreterResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;

    return run();
}
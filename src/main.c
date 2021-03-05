#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "common.h"

int main(int argc, char** argv) {
    Chunk chunk;
    init_vm();
    init_chunk(&chunk);

    for (int i = 0; i < 0xff + 2; i++) {
        write_constant(&chunk, i, 0);
    }
    write_chunk(&chunk, OP_RETURN, 1);
    interpret(&chunk);

    free_chunk(&chunk);
    free_vm();
    return 0;
}
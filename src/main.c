#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "common.h"

int main(int argc, char** argv) {
    Chunk chunk;
    init_vm();
    init_chunk(&chunk);

    // -((1.2 + 3.4) / 4.6)
    write_constant(&chunk, 1.2, 0);
    write_constant(&chunk, 3.4, 0);
    write_chunk(&chunk, OP_ADD, 0);
    write_constant(&chunk, 4.6, 0);
    write_chunk(&chunk, OP_DIVIDE, 0);
    write_chunk(&chunk, OP_NEGATE, 0);
    write_chunk(&chunk, OP_RETURN, 1);

    interpret(&chunk);

    free_chunk(&chunk);
    free_vm();
    return 0;
}
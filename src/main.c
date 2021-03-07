#include "vm.h"
#include "chunk.h"
#include "debug.h"
#include "common.h"

int main(int argc, char** argv) {
    Chunk chunk;
    init_vm();
    init_chunk(&chunk);

    write_constant(&chunk, 2.78, 0);
    write_constant(&chunk, 3.14, 1);
    write_chunk(&chunk, OP_RETURN, 2);

    interpret(&chunk);

    free_chunk(&chunk);
    free_vm();
    return 0;
}
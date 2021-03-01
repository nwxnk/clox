#include "chunk.h"
#include "debug.h"
#include "common.h"

int main(int argc, char** argv) {
    Chunk chunk;
    init_chunk(&chunk);

    write_chunk(&chunk, OP_RETURN, 0);
    disassemble_chunk(&chunk, "test chunk");

    free_chunk(&chunk);
    return 0;
}
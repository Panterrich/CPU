#include "Disassembler/Disassembler.h"

//============================================================

int main(int argc, const char* argv[])
{
    FILE* bytecode = fopen(argv[1], "rb");

    if (bytecode == NULL)
    {
        printf("File is not found, ERROR\n");
        fflush(stdout);
        abort();
    }

    char* buffer = Bytecode_buffer(bytecode);

    fclose(bytecode);

    FILE* output = fopen("Output.txt", "wb");

    Disassembling(buffer, output);

    free(buffer);
    fclose(output);
}


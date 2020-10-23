#include "CPU/CPU.h"

//======================================================

int main(int argc, char* argv[])
{
    FILE* code = fopen(argv[1], "rb");

    if (code == nullptr)
    {
        printf("File is not found, ERROR");
        abort();
    }

    struct CPU proc = {};
    CPU_construct(&proc, code);

    fclose(code);

    Processing(&proc);

    CPU_destruct(&proc);
    return 0;
}


#include "CPU.h"

//==============================================================

void CPU_construct(struct CPU* proc, FILE* file)
{
    assert(proc != nullptr);
    assert(file != nullptr);

    STACK_CONSTUCT(&(proc->stk), 1);

    unsigned char buffer[18] = {};

    fread(buffer, sizeof(unsigned char), 18, file);
    Verification_code(buffer);

    proc->size  = *(size_t*)(&buffer[2]);
    proc->n_cmd = *((size_t*)(&buffer[10]));

    proc->bytecode = (unsigned char*) calloc(proc->size, sizeof(unsigned char));
    proc->rip = 0;

    fread(proc->bytecode, sizeof(unsigned char), proc->size, file);

    for (int i = 0; i < 4; ++i)
    {
        (proc->registers)[i] = NAN;
    }
}

void Processing(struct CPU* proc)
{
    assert(proc != nullptr);

    for (size_t i = 0; i < proc->n_cmd; ++i)
    {
        Operation(proc, (proc->bytecode)[proc->rip]);
    }
}

void Operation(struct CPU* proc, int cmd)
{
    assert(proc != nullptr);
    assert(cmd != -1);

    switch (cmd)
    {
    #define DEF_COMMAND(name, number, hash, argc, code) case CMD_##name: code; break;
    #define DEF_MOD(name, number)
    #define DEF_REG(name, number, hash)

    #include "../libr/command.h"

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG

    default:
        printf("Unknown command, ERROR\n");
        printf("Code: %X \n", cmd);
        break;
    }
}

void CPU_destruct(struct CPU* proc)
{
    assert(proc != nullptr);

    free(proc->bytecode);
    
    Stack_destruct(&proc->stk);

    proc->n_cmd = -1;
    proc->rip = -1;
    proc->size = -1;

    RRAX = NAN;
    RRBX = NAN;
    RRCX = NAN;
    RRDX = NAN;
}

void Verification_code(unsigned char* buffer)
{
    assert(buffer != nullptr);

    if (Version != buffer[0])
    {
        printf("WARNING: assembler version doesn't match CPU verision\n");
        printf("Assembler verison: %d\n", buffer[0]);
        printf("CPU verison: %d\n", Version);
    }

    if (Signature != buffer[1])
    {
        printf("ERROR: assembler signature  doesn't match CPU signature\n");
        printf("Assembler signature: %X\n", buffer[1]);
        printf("CPU signature: %X\n", Signature);
        abort();
    }
}


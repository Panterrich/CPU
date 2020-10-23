#include "../libr/Stack.h"
#include "../libr/DSL.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
//==============================================================

const char Version = 3;
const unsigned char Signature = 0xDA;  

//==============================================================
struct CPU
{
    struct Stack stk;

    unsigned char* bytecode;
    size_t rip;

    size_t size;
    size_t n_cmd;

    element_t registers[4];
};

enum Commands 
{
    #define DEF_COMMAND(name, number, hash, argc, code) CMD_##name = number,
    #define DEF_MOD(name, number)
    #define DEF_REG(name, number, hash)

    #include "../libr/command.h"

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG 
};

enum Mods
{
    #define DEF_COMMAND(name, number, hash, argc, code)
    #define DEF_MOD(name, number) name = number,
    #define DEF_REG(name, number, hash) 

    #include "../libr/command.h"

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG 
};

enum Registers
{
    #define DEF_COMMAND(name, number, hash, argc, code)
    #define DEF_MOD(name, number)
    #define DEF_REG(name, number, hash) REG_##name = number,

    #include "../libr/command.h"

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG 
};

//=====================================================

void CPU_construct(struct CPU* proc, FILE* file);

void CPU_destruct(struct CPU* proc);

void Processing(struct CPU* proc);

void Operation(struct CPU* proc, int cmd);

void Verification_code(unsigned char* buffer);

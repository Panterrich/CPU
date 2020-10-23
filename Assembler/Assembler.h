#include "../libr/Onegin.h"
#include "../libr/Stack.h"
#include "../libr/DSL.h"
#include <assert.h>
#include <string.h>
#include <math.h>

//============================================================

const char Version = 3;
const unsigned char Signature = 0xDA;  

//============================================================

void Compiletion(struct Text* input_text, FILE* listing, FILE* code);

int Code_cmd(const char* cmd);

int Arg_command(int cmd);

unsigned int Hash(const char* cmd);

void Str_uppercase(char* string);

int Code_reg(const char* reg);

//============================================================

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

//=============================================================
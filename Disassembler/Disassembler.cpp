#include "Disassembler.h"

//========================================================================

char* Bytecode_buffer(FILE* file)
{
    assert(file != nullptr);

    size_t size = Size_file(file);

    char* buffer = (char*) calloc(size, sizeof(char));

    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), size, file);

    return buffer;
};

void Disassembling(char* buffer, FILE* output)
{
    assert(buffer != nullptr);
    assert(output != nullptr);

    fprintf(output, "Disassembler version: %d\n"
                    "Disassembler signature: %X\n\n\n"
                    "Code version: %d\n"
                    "Code signature: %X\n", 
                    Version, Signature, buffer[0], (unsigned char)buffer[1]);

    size_t count_cmd = *(size_t*)(&buffer[10]);
    size_t rip = 18;

    fprintf(output, "  Address  Number cmd     Code     Text\n");

    for (size_t num_current_cmd = 0; num_current_cmd < count_cmd; ++num_current_cmd)
    {   
        char cmd = buffer[rip];

        fprintf(output, "%9p %11d %8d ", &buffer[rip] - buffer, num_current_cmd + 1, cmd);

        if (Unknown_command(cmd))
        {
            fprintf(output, "Unknown command\n");
            printf("WARNING: UNKNOWN COMMAND - Number current command %d: %s", num_current_cmd, cmd);
            continue;
        }

        fprintf(output, "%8s ", name_code(cmd));

        if (Argument_command(cmd))
        {
            switch (buffer[rip + 1])
            {
                case MOD_EMPTY:
                    rip += sizeof(char);
                    break;

                case MOD_DOUBLE: 
                    fprintf(output, "%lg", *(element_t*)(&buffer[rip + 2]));
                    rip += sizeof(char) + sizeof(element_t);
                    break;

                case MOD_REG:
                    switch (buffer[rip + 2])
                    {
                        #define DEF_COMMAND(name, number, hash, argc, code)
                        #define DEF_MOD(name, number)
                        #define DEF_REG(name, num, hash)      \
                            case num:                         \
                                fprintf(output, "%s", #name); \
                                rip += 2 * sizeof(char);      \
                                break;

                        #include "../libr/command.h"

                        #undef DEF_COMMAND
                        #undef DEF_MOD
                        #undef DEF_REG

                        default:
                            fprintf(output, "unknown register");
                            printf("WARNING: UNKNOWN REGISTER - CMD: %s, REG: %d", name_code(cmd), buffer[rip + 2]);

                            rip += 2 * sizeof(char);

                            break;
                    }
                    break;

            default: 
                fprintf(output, "unknown mode");
                printf("WARNING: UNKNOWN MODE - CMD: %s, MODE: %d", name_code(cmd), buffer[rip + 1]);
                break;
            }
        }

        ++rip;
        
        fprintf(output, "\n");
    }
}

int Unknown_command(char cmd)
{
    #define DEF_COMMAND(Name, Num, Hash, Argc, code) case Num: return 0;
    #define DEF_MOD(name, number)
    #define DEF_REG(name, number, hash)

    switch (cmd)
    {
    #include "../libr/command.h"

    default:
        return 1;
    }

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG
}

const char* name_code(char cmd)
{
    #define DEF_COMMAND(name, num, hash, argc, code) case num: return #name;
    #define DEF_MOD(name, number)
    #define DEF_REG(name, number, hash)

    switch (cmd)
    {
    #include "../libr/command.h"
    }

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG

    return "\0";
}

int Argument_command(char cmd)
{
    #define DEF_COMMAND(name, num, hash, argc, code) case num: return argc;
    #define DEF_MOD(name, number)
    #define DEF_REG(name, number, hash)

    switch (cmd)
    {
    #include "../libr/command.h"
    }

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG

    return 0;
}
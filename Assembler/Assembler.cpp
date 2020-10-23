#include "Assembler.h"

//========================================================================

void Compiletion(struct Text* input_text, FILE* listing, FILE* code)
{
    assert(input_text != nullptr);
    assert(listing != nullptr);
    assert(code != nullptr);

    size_t count_cmd = 0;
    size_t size = 0;
    char* buffer = (char*) calloc(input_text->n_lines, sizeof(element_t) + 2 * sizeof(char));
    char* pointer = buffer;

    fprintf(listing, "  Adress   Number     Code     Text\n");

    for (int index_strings = 0; index_strings < input_text->n_lines; ++index_strings)
    {
        char* command = (char*) calloc(input_text->lines[index_strings].len, sizeof(char));
        char* reg = (char*) calloc(input_text->lines[index_strings].len, sizeof(char));
        double value = NAN;
        

        sscanf((input_text->lines)[index_strings].str, " %s ", command);
        Str_uppercase(command);

        char cmd = Code_cmd(command);

        if (cmd != -1)
        {   
            ++count_cmd;
            *pointer = cmd;
            ++pointer;

            if (Arg_command(cmd))
            {
                if (sscanf((input_text->lines)[index_strings].str, " %*s %lg", &value) != 1)
                {
                    sscanf((input_text->lines)[index_strings].str, " %*s %s", reg);
                    Str_uppercase(reg);

                    char number_register = Code_reg(reg);

                    if (number_register != -1)
                    {
                        *pointer = (char)MOD_REG;
                        ++pointer;

                        *pointer = number_register;
                        ++pointer;

                        fprintf(listing, "%8p %8lu %3lu %1lu %2lu %8s %s\n", pointer - buffer, count_cmd, cmd, MOD_REG, number_register, command, reg);
                    }
                    
                    else if ((cmd == CMD_POP) || (cmd == CMD_PURGE) || (cmd == CMD_IN) || (cmd == CMD_OUT))
                    {
                        *pointer = (char)MOD_EMPTY;
                        ++pointer;

                        fprintf(listing, "%8p %8lu %5lu %2lu %8s\n", pointer - buffer, count_cmd, cmd, MOD_EMPTY, command);
                    }

                    else
                    {
                        *pointer = (char)MOD_REG;
                        ++pointer;

                        fprintf(listing, "%8p %8lu %4lu %1lu Unknown mod or register %8s %s\n", count_cmd, cmd, MOD_REG, command, reg);
                    }
             
               }

                else
                {
                    *pointer = MOD_DOUBLE;
                    ++pointer;

                    *((element_t*)pointer) = value;
                    pointer += sizeof(element_t);
                
                    fprintf(listing, "%8p %8lu %6lu %1lu %8s %lg\n", pointer - buffer, count_cmd, cmd, MOD_DOUBLE, command, value);
                }
            }

            else
            {
                fprintf(listing, "%8p %8lu %8lu %8s\n", pointer - buffer, count_cmd, cmd, command);
            }
         }

        else
        {
            fprintf(listing, "%8p %8lu Unknown command %8s\n", pointer - buffer, count_cmd, command);    
        }

        free(command);
        command = nullptr;
    }

    size = pointer - buffer;

    fwrite(&Version,   sizeof(char),          1, code);
    fwrite(&Signature, sizeof(unsigned char), 1, code);
    fwrite(&size,      sizeof(size_t),        1, code);
    fwrite(&count_cmd, sizeof(size_t),        1, code);
    fwrite(buffer,     sizeof(char),       size, code);

    free(buffer);
    buffer = nullptr;
}

int Code_cmd(const char* cmd)
{
    assert(cmd != nullptr);

    int hcmd = Hash(cmd);

    #define DEF_COMMAND(name, number, hash, argc, code) if (hcmd == hash) return CMD_##name;
    #define DEF_MOD(name, number)
    #define DEF_REG(name, number, hash)

    #include "../libr/command.h"

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG 

    /*else*/ return -1;
}

int Code_reg(const char* reg)
{
    assert(reg != nullptr);

    int hreg = Hash(reg);

    #define DEF_COMMAND(name, number, hash, argc, code)
    #define DEF_MOD(name, number) 
    #define DEF_REG(name, number, hash) if (hreg == hash) return REG_##name;

    #include "../libr/command.h"

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG 

    /*else*/ return -1;  
}

int Arg_command(int cmd)
{
    assert(cmd != -1);

    #define DEF_COMMAND(name, number, hash, argc, code) if ((cmd == CMD_##name) && (argc >= 1)) return 1;
    #define DEF_MOD(name, number) 
    #define DEF_REG(name, number, hash)

    #include "../libr/command.h"

    #undef DEF_COMMAND
    #undef DEF_MOD
    #undef DEF_REG 

    /*else*/ return 0;
}

void Str_uppercase(char* string)
{
    assert(string != nullptr);

    int len = strlen(string);

    for (int i = 0; i < len; ++i)
    {
        string[i] = toupper(string[i]);
    }
}

unsigned int Hash(const char* cmd)
{
    assert(cmd != nullptr);

    unsigned int hash = 0;
    
    for (int i = 0; i < strlen(cmd); ++i)
    {
        hash += (unsigned char)(*(cmd + i));
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
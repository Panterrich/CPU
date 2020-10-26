#include "Assembler.h"

//========================================================================

int Compile_pass(struct Text* input_text, struct Label* labels)
{
    assert(input_text != nullptr);
    assert(labels     != nullptr);

    FILE* listing = fopen("ASM_listing.txt", "wb");
    FILE* code    = fopen("code.txt",        "wb");

    if (Compilation(input_text, listing, code, labels)) return 1;

    fclose(listing);
    fclose(code);

    return 0;
}

int Compilation(struct Text* input_text, FILE* listing, FILE* code, struct Label* labels)
{
    assert(input_text != nullptr);
    assert(listing    != nullptr);
    assert(code       != nullptr);
    
    static size_t count_label = 0;

    size_t count_cmd   = 0;
    size_t size        = 0;

    char* buffer  = (char*) calloc(input_text->n_lines, sizeof(element_t) + 2 * sizeof(char));
    char* pointer = buffer;

    fprintf(listing, 
        "Assembler version: %d\n"
        "Assembler signature: %X\n\n\n"
        "  Adress   Number     Code     Text\n", Version, Signature);

    for (int index_strings = 0; index_strings < input_text->n_lines; ++index_strings)
    {   
        char* label   = (char*) calloc((input_text->lines)[index_strings].len, sizeof(char));
        char* command = (char*) calloc((input_text->lines)[index_strings].len, sizeof(char));
        char* reg     = (char*) calloc((input_text->lines)[index_strings].len, sizeof(char));

        char*  current_pointer = (input_text->lines)[index_strings].str;
        size_t current_lshift = 0;
        size_t current_rshift = 0;

        double value = NAN;

        if (Next_word(current_pointer, label,   &current_lshift, &current_rshift))
        {
            if (label[current_rshift - current_lshift - 1] == ':')
            {
                label[current_rshift - current_lshift - 1] = '\0';

                Str_uppercase(label);
                unsigned int hlabel = Hash(label);

                size_t count_no_matches = 0;
                
                for (int i = 0; i < count_label; ++i)
                {
                    if (hlabel != labels[i].hash)
                    {
                        ++count_no_matches;
                    }
                    
                    else 
                    {
                        break;
                    }
                }

                if (count_no_matches == count_label)
                {
                    labels[count_label].hash    = hlabel; 
                    labels[count_label].address = pointer - buffer;
                    ++count_label;
                }
                
                current_pointer += current_rshift;
            }
        }

        if (Next_word(current_pointer, command, &current_lshift, &current_rshift))
        {
            Str_uppercase(command);
            char cmd = Code_cmd(command);

            if (cmd != -1)
            {   
                ++count_cmd;
                *pointer = cmd;
                ++pointer;

                if (Arg_command(cmd))
                {
                    if (sscanf(current_pointer, " %*s %lg", &value) != 1)
                    {
                        sscanf(current_pointer, " %*s %s", reg);
                        Str_uppercase(reg);

                        if (reg[0] == ':')
                        {
                            if ((cmd >= 30) && (cmd < 38))
                            {
                                unsigned int label_hash = Hash(reg + 1);
                                unsigned int dummy = 0;

                                for (int i = 0; i < count_label; ++i)
                                {
                                    if (label_hash == labels[i].hash)
                                    {
                                        *pointer = MOD_POINTER;
                                        ++pointer;

                                        *((size_t*)pointer) = labels[i].address;
                                        pointer += sizeof(size_t);

                                        dummy = 1;

                                        fprintf(listing, "%#8x %8lu %3lu %lu %2lu %8s :%s \n", 
                                                pointer - buffer - 2 * sizeof(char) - sizeof(size_t),
                                                count_cmd, cmd, MOD_POINTER, labels[i].address, command, reg + 1);
                                    }
                                }

                                if (dummy == 0)
                                {
                                    *pointer = MOD_POINTER;
                                    ++pointer;

                                    *((size_t*)pointer) = -1;
                                    pointer += sizeof(size_t);
                                }
                            }

                            else 
                            {
                                fprintf(listing, "%#8x %8lu %5lu This command don't use label: %8s %s \n", 
                                        pointer - buffer - sizeof(char),
                                        count_cmd, cmd, command, reg + 1);
                                                
                                printf("ERROR: This command don't use label: %8s %s \n", command, reg + 1);

                                return 1;
                            }

                            Free_mem(command, reg, label);
                            continue;
                        }

                        char number_register = Code_reg(reg);

                        if (number_register != -1)
                        {
                            *pointer = (char)MOD_REG;
                            ++pointer;

                            *pointer = number_register;
                            ++pointer;

                            fprintf(listing, "%#8x %8lu %3lu %1lu %2lu %8s %s\n", 
                                    pointer - buffer - 3 * sizeof(char),
                                    count_cmd, cmd, MOD_REG, number_register, command, reg);
                        }
                        
                        else if ((cmd == CMD_POP) || (cmd == CMD_PURGE) || (cmd == CMD_IN) || (cmd == CMD_OUT))
                        {
                            *pointer = (char)MOD_EMPTY;
                            ++pointer;

                            fprintf(listing, "%#8x %8lu %5lu %2lu %8s\n",
                                    pointer - buffer - 2 * sizeof(char),
                                    count_cmd, cmd, MOD_EMPTY, command);
                        }

                        else
                        {
                            *pointer = (char)MOD_REG;
                            ++pointer;

                            fprintf(listing, "%#8x %8lu %4lu %1lu Unknown mod or register %8s %s\n",
                                    pointer - buffer - 2 * sizeof(char),  
                                    count_cmd, cmd, MOD_REG, command, reg);
                            printf("ERROR: Unknown mod or register: %8s %s \n", command, reg);

                            return 1;
                        }
                }

                    else
                    {
                        *pointer = MOD_DOUBLE;
                        ++pointer;

                        *((element_t*)pointer) = value;
                        pointer += sizeof(element_t);
                    
                        fprintf(listing, "%#8x %8lu %3lu %1lu %2lg %8s %lg\n", 
                                pointer - buffer - sizeof(element_t) - 2 * sizeof(char),
                                count_cmd, cmd, MOD_DOUBLE, value,  command, value);
                    }
                }

                else
                {
                    fprintf(listing, "%#8x %8lu %8lu %8s\n", 
                            pointer - buffer - sizeof(char),
                            count_cmd, cmd, command);
                }
            }

            else
            {
                fprintf(listing, "%#8x %8lu Unknown command %8s\n", 
                        pointer - buffer - sizeof(char),
                        count_cmd, command);
                printf("ERROR: Unknown command: %8s\n", command);

                return 1;
            }
        }

        Free_mem(command, reg, label);
    }

    size = pointer - buffer;

    fwrite(&Version,   sizeof(char),          1, code);
    fwrite(&Signature, sizeof(unsigned char), 1, code);
    fwrite(&size,      sizeof(size_t),        1, code);
    fwrite(&count_cmd, sizeof(size_t),        1, code);
    fwrite(buffer,     sizeof(char),       size, code);

    free(buffer);
    buffer = nullptr;

    return 0;
}
        
void Free_mem(char* command, char* reg, char* label)
{
    free(command);
    free(reg);
    free(label);

    command = nullptr;
    reg     = nullptr;
    label   = nullptr;
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

int Next_word(char* begin, char* word, size_t* lshift, size_t* rshift)
{
    *lshift = 0;
    *rshift = 0;

    sscanf(begin, " %n%s%n", lshift, word, rshift);

    if (*rshift == 0) 
    {
        return 0;
    }

    else 
    {
        return 1;
    }
}

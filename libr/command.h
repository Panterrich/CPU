DEF_COMMAND(HLT,  0,  0x1e3edd86, 0, 
    {
        return 1;
    })

DEF_COMMAND(DUMP, 7,  0x39c100dd, 0, 
    {
        Stack_dump(stdout, &(proc->stk));

        ++RIP;
    })

DEF_COMMAND(PURGE, 4, 0x6fee0dcd, 1, 
    {
        if (BYTECODE[RIP + 1] == MOD_EMPTY)
        {
            for (int i = 0; i < 4; ++i)
            {
                REGS[i] = Poison;
            }

            RIP += 2 * sizeof(char);
        }

        else if (BYTECODE[RIP + 1] == MOD_REG)
        {
            REGS[BYTECODE[RIP + 2] - 'a'] = Poison;

            RIP += 3 * sizeof(char);
        }
    })

DEF_COMMAND(IN,   10, 0x2d0b8777, 1, 
    {   
        printf("IN: ");

        element_t element = 0;
        scanf("%lg", &element);
        
        if (BYTECODE[RIP + 1] == MOD_EMPTY)
        {
            PUUSH(element);

            RIP += 2 * sizeof(char);
        }

        else if (BYTECODE[RIP + 1] == MOD_REG)
        {
            REGS[BYTECODE[RIP + 2] - 'a'] = element;

            RIP += 3 * sizeof(char);
        }

    })

DEF_COMMAND(OUT,  11, 0xcd3efa96, 1, 
    {
        if (BYTECODE[RIP + 1] == MOD_EMPTY)
        {
            printf("OUT: %lg \n", POOP());

            RIP += 2 * sizeof(char);
        }

        else if (BYTECODE[RIP + 1] == MOD_REG)
        {
            printf("Out: %lg \n", REGS[BYTECODE[RIP + 2] - 97]);

            RIP += 3 * sizeof(char);
        }

    })

DEF_COMMAND(PUSH, 20, 0x12e2afe4, 1, 
    {
        if (BYTECODE[RIP + 1] == MOD_DOUBLE)
        {
            PUUSH(*(element_t*)(&BYTECODE[RIP + 2]));

            RIP += sizeof(element_t) + 2 * sizeof(char); 
        }

        else if (BYTECODE[RIP + 1] == MOD_REG)
        {
            PUUSH(REGS[BYTECODE[RIP + 2] - 97]);

            RIP += 3 * sizeof(char);
        }
    })

DEF_COMMAND(POP,  21, 0xa9527a55, 1, 
    {
        if (BYTECODE[RIP + 1] == MOD_EMPTY)
        {
            POOP();

            RIP += 2 * sizeof(char); 
        }

        else if (BYTECODE[RIP + 1] == MOD_REG)
        {
            REGS[BYTECODE[RIP + 2] - 97] = POOP();
            
            RIP += 3 * sizeof(char);
        }
    })

DEF_COMMAND(NEG,  30, 0x3b1a041e, 0, 
    {
        element_t temp = POOP();
        PUUSH(-temp);

        ++RIP;
    })

DEF_COMMAND(ADD,  31, 0xc281d6b4, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_1 + temp_2);

        ++RIP;
    })
    
DEF_COMMAND(SUB,  32, 0x1aa20312, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_2 - temp_1);

        ++RIP;
    })

DEF_COMMAND(MUL,  33, 0xadd0b179, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_1 * temp_2);

        ++RIP;
    })

DEF_COMMAND(DIV,  34, 0xe550a8d4, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_2 / temp_1);

        ++RIP;
    })

DEF_COMMAND(POW,  35, 0xe7a3f707, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(pow(temp_2, temp_1));

        ++RIP;
    })

DEF_COMMAND(SIN,  40, 0xab2b99ba, 0, 
    {
        element_t temp = POOP();
        PUUSH(sin(temp));

        ++RIP;
    })

DEF_COMMAND(COS,  41, 0x371862fa, 0, 
    {
        element_t temp = POOP();
        PUUSH(cos(temp));

        ++RIP;
    })

DEF_COMMAND(TG,   42, 0xeda6fc83, 0, 
    {
        element_t temp = POOP();
        PUUSH(tan(temp));

        ++RIP;
    })

DEF_COMMAND(CTG,  43, 0x3bd5719c, 0, 
    {
        element_t temp = POOP();
        PUUSH(1.0 / tan(temp));

        ++RIP;
    })

DEF_COMMAND(SQRT, 44, 0x9cca14cb, 0, 
    {
        element_t temp = POOP();
        PUUSH(sqrt(temp));

        ++RIP;
    })

DEF_MOD(MOD_DOUBLE, 1)
DEF_MOD(MOD_REG,    2)
DEF_MOD(MOD_EMPTY,  3)

DEF_REG(RAX, 97,  0xf49435a3)
DEF_REG(RBX, 98,  0x4b98744e)
DEF_REG(RCX, 99,  0xfb88d770)
DEF_REG(RDX, 100, 0xb178c235)
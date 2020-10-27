DEF_COMMAND(HLT,  0,  0x1e3edd86, 0, 
    {
        return 1;
    })

DEF_COMMAND(DUMP, 7,  0x39c100dd, 0, 
    {
        Stack_dump(stdout, &(proc->stk));
        Stack_dump(stdout, &(proc->call_stk));

        RIP += size_cmd;
    })

DEF_COMMAND(PURGE, 4, 0x6fee0dcd, 1, 
    {
        if (CODE_MOD == MOD_EMPTY)
        {
            for (int i = 0; i < 4; ++i)
            {
                REGS[i] = Poison;
            }

            RIP += size_cmd + size_mod;
        }

        else if (CODE_MOD == MOD_REG)
        {
            REGS[NUM_REG(0)] = Poison;

            RIP += size_cmd + size_mod + size_reg;
        }
    })

DEF_COMMAND(IN,   10, 0x2d0b8777, 1, 
    {   
        printf("IN: ");

        element_t element = 0;
        scanf("%lg", &element);
        
        if (CODE_MOD == MOD_EMPTY)
        {
            PUUSH(element);

            RIP += size_cmd + size_mod;
        }

        else if (CODE_MOD == MOD_REG)
        {
            REGS[NUM_REG(0)] = element;

            RIP += size_cmd + size_mod + size_reg;
        }

    })

DEF_COMMAND(OUT,  11, 0xcd3efa96, 1, 
    {
        if (CODE_MOD == MOD_EMPTY)
        {
            printf("OUT: %lg \n", POOP());

            RIP += size_cmd + size_mod;
        }

        else if (CODE_MOD == MOD_REG)
        {
            printf("Out: R%cX = %lg \n", NUM_REG(0) + 'A' , REGS[NUM_REG(0)]);

            RIP += size_cmd + size_mod + size_reg;
        }

    })

DEF_COMMAND(PUSH, 20, 0x12e2afe4, 1, 
    {
        if (CODE_MOD == MOD_DOUBLE)
        {
            PUUSH(VALUE(0));

            RIP += size_cmd + size_mod + size_arg; 
        }

        else if (CODE_MOD == MOD_REG)
        {
            PUUSH(REGS[NUM_REG(0)]);

            RIP += size_cmd + size_mod + size_reg;
        }
    })

DEF_COMMAND(POP,  21, 0xa9527a55, 1, 
    {
        if (CODE_MOD == MOD_EMPTY)
        {
            POOP();

            RIP += size_cmd + size_mod; 
        }

        else if (CODE_MOD == MOD_REG)
        {
            REGS[NUM_REG(0)] = POOP();
            
            RIP += size_cmd + size_mod + size_reg;
        }
    })

DEF_COMMAND(JMP, 30, 0xed5354f0, 1, 
    {
        RIP = SHIFT_ON_LABEL;
    })

DEF_COMMAND(JA, 31, 0x5d53e6f3, 1, 
    {
        element_t temp1 = POOP();
        element_t temp2 = POOP();

        if (temp2 > temp1)
        {
            RIP = SHIFT_ON_LABEL;
        }

        else
        {
            RIP += size_cmd + size_mod + size_lbl;
        }
    })

DEF_COMMAND(JAE, 32, 0xfa010a1d, 1, 
    {
        element_t temp1 = POOP();
        element_t temp2 = POOP();

        if (temp2 >= temp1)
        {
            RIP = SHIFT_ON_LABEL;
        }

        else 
        {
            RIP += size_cmd + size_mod + size_lbl;
        }
    })

DEF_COMMAND(JB, 33, 0x15dbd7ec, 1, 
    {
        element_t temp1 = POOP();
        element_t temp2 = POOP();

        if (temp2 < temp1)
        {
            RIP = SHIFT_ON_LABEL;
        }

        else 
        {
            RIP += size_cmd + size_mod + size_lbl;
        }
    })

DEF_COMMAND(JBE, 34, 0x70796a7c, 1, 
    {
        element_t temp1 = POOP();
        element_t temp2 = POOP();

        if (temp2 <= temp1)
        {
            RIP = SHIFT_ON_LABEL;
        }

        else 
        {
            RIP += size_cmd + size_mod + size_lbl;
        }
    })

DEF_COMMAND(JE, 35, 0x4326b285, 1, 
    {
        element_t temp1 = POOP();
        element_t temp2 = POOP();

        if (temp2 == temp1)
        {
            RIP = SHIFT_ON_LABEL;
        }

        else 
        {
            RIP += size_cmd + size_mod + size_lbl;
        }
    })

DEF_COMMAND(JNE, 36, 0x9cce3533, 1, 
    {
        element_t temp1 = POOP();
        element_t temp2 = POOP();

        if (temp2 != temp1)
        {
            RIP = SHIFT_ON_LABEL;
        }

        else 
        {
            RIP += size_cmd + size_mod + size_lbl;
        }
    })

DEF_COMMAND(CALL, 37, 0xe3ed334c, 1, 
    {
        Stack_push(&CALL_STACK, (RIP + size_cmd + size_mod + size_lbl));

        RIP = SHIFT_ON_LABEL;

    })

DEF_COMMAND(RET, 38, 0x438f69b8, 0,
    {
        RIP = Stack_pop(&CALL_STACK);
    })

DEF_COMMAND(NEG,  40, 0x3b1a041e, 0, 
    {
        element_t temp = POOP();
        PUUSH(-temp);

        RIP += size_cmd;
    })

DEF_COMMAND(ADD,  41, 0xc281d6b4, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_1 + temp_2);

        RIP += size_cmd;
    })
    
DEF_COMMAND(SUB,  42, 0x1aa20312, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_2 - temp_1);

        RIP += size_cmd;
    })

DEF_COMMAND(MUL,  43, 0xadd0b179, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_1 * temp_2);

        RIP += size_cmd;
    })

DEF_COMMAND(DIV,  44, 0xe550a8d4, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(temp_2 / temp_1);

        RIP += size_cmd;
    })

DEF_COMMAND(POW,  45, 0xe7a3f707, 0, 
    {
        element_t temp_1 = POOP();
        element_t temp_2 = POOP();
        PUUSH(pow(temp_2, temp_1));

        RIP += size_cmd;
    })

DEF_COMMAND(SIN,  50, 0xab2b99ba, 0, 
    {
        element_t temp = POOP();
        PUUSH(sin(temp));

        RIP += size_cmd;
    })

DEF_COMMAND(COS,  51, 0x371862fa, 0, 
    {
        element_t temp = POOP();
        PUUSH(cos(temp));

        RIP += size_cmd;
    })

DEF_COMMAND(TG,   52, 0xeda6fc83, 0, 
    {
        element_t temp = POOP();
        PUUSH(tan(temp));

        RIP += size_cmd;
    })

DEF_COMMAND(CTG,  53, 0x3bd5719c, 0, 
    {
        element_t temp = POOP();
        PUUSH(1.0 / tan(temp));

        RIP += size_cmd;
    })

DEF_COMMAND(SQRT, 54, 0x9cca14cb, 0, 
    {
        element_t temp = POOP();
        PUUSH(sqrt(temp));

        RIP += size_cmd;
    })

DEF_MOD(MOD_DOUBLE,  1)
DEF_MOD(MOD_REG,     2)
DEF_MOD(MOD_EMPTY,   3)
DEF_MOD(MOD_POINTER, 4)

DEF_REG(RAX, 97,  0xf49435a3)
DEF_REG(RBX, 98,  0x4b98744e)
DEF_REG(RCX, 99,  0xfb88d770)
DEF_REG(RDX, 100, 0xb178c235)
DEF_REG(REX, 101, 0xcf2300e1)
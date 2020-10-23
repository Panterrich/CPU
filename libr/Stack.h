#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#define DOUBLE_T

#ifdef DOUBLE_T
    typedef double element_t;
    static const int code_t = 1;
    static const double Poison = NAN;
    static const char*  Poison_text = "NAN";
#endif

#ifdef INT_T
    typedef int element_t;
    static const int code_t = 2;
    static const int   Poison = 0xBADDED;
    static const char* Poison_text = "0xBADDED";
#endif

#ifdef CHAR_T
    typedef char element_t;
    static const int code_t = 3;
    static const char  Poison = '\0';
    static const char* Poison_text = "\\0";
#endif

//===================================================

#define STACK_CONSTUCT(stk, capacity) (stk)->name = #stk;               \
                                      Stack_construct(stk, capacity)

//===================================================

#define ASSERT_OK(a) if (Stack_ERROR(a))                                                                            \
                        {                                                                                           \
                            FILE* log = fopen("log.txt", "a");                                                      \
                            assert(log != nullptr);                                                                 \
                            fprintf(log, "ERROR: file %s line %d function %s\n", __FILE__, __LINE__, __FUNCTION__); \
                            Stack_dump(log, a);                                                                     \
                            abort();                                                                                \
                        }
//===================================================

#define case_of_switch(enum_const) case enum_const: return #enum_const;

//===================================================
typedef unsigned long long canary_t;

static const canary_t Canary = 0xBADDF00DDEADBEAF;
//===================================================

struct Stack
{
    canary_t canary_struct_left;

    const char* name;
    size_t size;
    size_t capacity;
    element_t* data;

    int error;
    unsigned int struct_hash;
    unsigned int stack_hash;

    canary_t canary_struct_right;
};

//===================================================

enum ERROR 
{
    SIZE_OUT_OF_CAPACITY = 1,
    OUT_OF_MEMORY,
    NEGATIVE_SIZE,
    NEGATIVE_CAPACITY,
    NULL_POINTER_TO_ARRAY,
    ARRAY_AND_STRUCTURE_POINTERS_MATCHED,
    REPEATED_CONSTRUCTION,
    WRONG_SIZE,
    NULL_POP,
    WRONG_CANARY_STRUCT_LEFT,
    WRONG_CANARY_STRUCT_RIGHT,
    WRONG_CANARY_ARRAY_LEFT,
    WRONG_CANARY_ARRAY_RIGHT,
    WRONG_STRUCT_HASH,
    WRONG_STACK_HASH,
    INVALID_PUSH,
    STACK_IS_DESTRUCTED
};

//===================================================

int Stack_is_destructed(struct Stack* stk);

void Stack_construct(struct Stack* stk, long capacity);

void Stack_push(struct Stack* stk, element_t element);

element_t Stack_pop(struct Stack* stk);

void Stack_reallocation_memory(struct Stack* stk);

void Stack_destruct(struct Stack* stk);

void Stack_reverse_reallocation_memory(struct Stack* stk);

void NULL_check(struct Stack* stk);

int Stack_ERROR(struct Stack* stk);

void Stack_dump(FILE* file, struct Stack* stk);

void Poison_filling(struct Stack* stk, size_t start, size_t end);

const char* Text_ERROR(struct Stack* stk);

void Print_array(FILE*file, struct Stack* stk);

int Comparator_poison(element_t element);

void Placing_canary(struct Stack* stk, void* temp);

unsigned int Struct_stack_HASHFAQ6(struct Stack* stk);

unsigned int Stack_HASHFAQ6(struct Stack* stk);

#endif

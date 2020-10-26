#ifndef DSL_HEADER
#define DSL_HEADER

#define PUUSH(item) Stack_push(&proc->stk, item)
#define POOP()      Stack_pop(&proc->stk)
#define CALL_STACK (proc->call_stk)
#define BYTECODE   (proc->bytecode)
#define REGS       (proc->registers)
#define RRAX       (proc->registers)[0]
#define RRBX       (proc->registers)[1]
#define RRCX       (proc->registers)[2]
#define RRDX       (proc->registers)[3]
#define RIP        (proc->rip)

#endif

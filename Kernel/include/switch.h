#include "list.h"
#define GET_ESP(esp,ebp)\
    asm("mov %%esp, %0" "\n\t" "mov %%ebp, %1"\
        : "=r" (esp), "=r" (ebp));
#define SET_ESP(esp,ebp)\
    asm("mov %0, %%esp" "\n\t" "mov %1,%%ebp"\
        :: "r" (esp), "r" (ebp));

#define STACK_SIZE 4096

typedef int (func_t)(int);

typedef void (func_tBis)(void *);

struct ctx_s{
    int esp;
    int ebp;
};

struct ctx_sBis{
    int esp;
    int ebp;
    func_tBis* f;
    void* args;
    char stack[STACK_SIZE];
    int started;
};

struct ctx_sLinked{
    int esp;
    int ebp;
    func_tBis* f;
    void* args;
    char stack[STACK_SIZE];
    int started;
    int status;
    int id;
    struct list_head handle;
    char name[10*sizeof(char)];
};
extern struct ctx_sLinked* currentCtx;
extern struct ctx_sLinked contexts[10];
extern struct list_head archives;


int init_ctx(struct ctx_sBis *ctx, func_tBis f, void *args);
void switch_to_ctx(struct ctx_sLinked *ctx);
int create_ctx(func_tBis f, void *args, char* name);
struct ctx_sLinked* findContext();
void yield();
void setupContexts();
void start_sched();
void handler();
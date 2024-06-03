typedef unsigned int size_t;

#include "minilib.h"
#include "switch.h"
#include "list.h"
#include "idt.h"
#define FREE 0
#define INIT 1
#define TIMER_IRQ 0

struct ctx_sLinked* currentCtx;
struct ctx_sLinked contexts[10];

void setupContexts(){
    for (int i=0; i<10; i++){
        contexts[i].status= FREE;
    }
}

LIST_HEAD(archives);

int init_ctx(struct ctx_sBis *ctx, func_tBis f, void *args){
    ctx->f = f;
    ctx->args = args;
    ctx->esp = (int)ctx->stack+STACK_SIZE-4;
    ctx->ebp = (int)ctx->stack+STACK_SIZE-4;
    ctx->started = 0;
    return 0;
}

void switch_to_ctx(struct ctx_sLinked *ctx){
    // Initialiser les context en passant par leurs pointeurs
    int myesp,myebp;
    GET_ESP(myesp,myebp)
    if (currentCtx != NULL){
        currentCtx->esp = myesp;
        currentCtx->ebp = myebp;
    }
    if(ctx->started == 0){
        ctx->started = 1;
        currentCtx = ctx;
        SET_ESP(ctx->esp,ctx->ebp);
        end_of_interrupt();
        irq_enable();
        currentCtx->f(ctx->args);
    }
    else{
        currentCtx = ctx;
        SET_ESP(ctx->esp,ctx->ebp);
    }
    irq_enable();
}

int create_ctx(func_tBis f, void *args, char* name){

    struct ctx_sLinked *p = findContext();
    p->f = f;
    p->args = args;
    p->esp = (int)p->stack+STACK_SIZE-4;
    p->ebp = (int)p->stack+STACK_SIZE-4;
    p->started = 0;
    p->status = INIT;
    for (int j = 0; j<5;j++){
        p->name[j]=name[j];
    }
    INIT_LIST_HEAD(&p->handle);
    list_add_tail(&p->handle, &archives);
    return 0;
}

struct ctx_sLinked* findContext(){
    int i = 0;
    while(i<10 && contexts[i].status !=FREE){

        i++;
    }
    if (i == 10){
        return NULL;
    }
    return &contexts[i];
}

void yield(){
    struct ctx_sLinked* first = list_first_entry(&archives, struct ctx_sLinked,handle);
    list_move_tail(&first->handle,&archives);

    switch_to_ctx(list_first_entry(&archives,struct ctx_sLinked,handle));
}

void handler(){
    irq_disable();
    yield();
}

void start_sched(){
    idt_setup_irq_handler(TIMER_IRQ,handler);
}
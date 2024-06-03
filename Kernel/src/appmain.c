#include "minilib.h"
#include "switch.h"
#include "list.h"
#include "ioport.h"
#include "keyboard.h"
#define WAIT(x)                             \
    do                                      \
    {                                       \
        for (int i =0;i<10000*x;i++);       \
                                            \
    }while(0)                               \

struct ctx_s context;
static int res;

struct ctx_sLinked ctxPing,ctxPong;




void simpleAdd(){
    int a = 1;
    int b = 2;
    a = a + b;
    int myesp,myebp;
    GET_ESP(myesp,myebp)
    puthex(myesp);
    puthex(myebp);
};



int try(struct ctx_s *pctx, func_t *f, int arg){
    GET_ESP(pctx->esp,pctx->ebp)
    puthex(pctx->esp);
    puthex(pctx->ebp);
    return f(arg);
}

int throw(struct ctx_s *pctx, int r){
    int local = 0;
    puts("Entering throw\n");
    int myesp,myebp;
    GET_ESP(myesp,myebp)
    puthex(myesp);
    puthex(myebp);
    res = r;
    SET_ESP(pctx->esp,pctx->ebp);
    // recupere les registres dans la structe et replace dans les registres
    return res;
}

int funcThrow(int arg){
    int myesp,myebp;
    GET_ESP(myesp,myebp)
    puthex(myesp);
    puthex(myebp);
    int r = throw(&context,arg);
    puts("r in funcThrow");
    puthex(r);
    return r;
}



void f_ping(void *args){
    while(1){
        putc('A');
        WAIT(1000);
        //yield();
        putc('B');
        WAIT(1000);
        //yield();
        putc('C');
        WAIT(1000);
        //yield();
    }
}

void f_pong(void *args){
    while(1){
        putc('1');
        WAIT(1000);
        //yield();
        putc('2');
        WAIT(1000);
        //yield();
    }
}

void infiniteWrite(){
    while(1){
        putc('A');
        WAIT(3000);
    }
}

void app_main()
{
    // int myesp,myebp;
    // GET_ESP(myesp,myebp)
    // puthex(myesp);
    // puthex(myebp);
    // puts("Entering Add\n");
    // simpleAdd();
    // puts("Entering try\n");
    // int ans = try(&context,funcThrow,0);
    // puts("After the try/throw\n");
    // puthex(context.esp);
    // puthex(context.ebp);
    // puts("Result : ");
    // puthex(ans);
    // init_ctx(&ctxPing,f_ping,NULL);
    // init_ctx(&ctxPong,f_pong,NULL);
    // switch_to_ctx(&ctxPing);


    /* setupContexts();
    create_ctx(f_ping, NULL,"ping");
    create_ctx(f_pong, NULL,"pong");
    start_sched(); */

    /* puts("Hello World!\n");
    while(1){
        putc(_inb(0x60))
        WAIT(1000);
        // print des scancode de chaque touche mais ce n'est pas mappé au clavier azerty
    } */
    struct sem keyboardSem;
    init_sem(&keyboardSem,1);

    setupContexts();
    struct ctx_sLinked keyboardContext;
    create_ctx(getc, NULL,"keyb");
    struct ctx_sLinked permanentCtx;
    create_ctx(infiniteWrite, NULL, "perm");

    
    start_sched();
    /* while(1){
        writeKeyboard();
        WAIT(1000);
        
    } */
    
};




#include "switch.h"
#include "list.h"
#include "idt.h"

struct sem{
    int cpt;
    struct list_head waitList;
};

extern struct sem keyboardSem;

void init_sem(struct sem*,unsigned int);
void sem_down(struct sem *sem);
void sem_up(struct sem *sem);
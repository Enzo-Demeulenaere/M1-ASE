#include "semaphore.h"
#include <stddef.h>

void init_sem(struct sem *sem, unsigned int val){
    
    sem->cpt = val;
    INIT_LIST_HEAD(&sem->waitList);
}

void sem_down(struct sem *sem){
    sem->cpt = sem->cpt -1;
    if (sem->cpt <0){
        list_del(&currentCtx->handle);
        list_add(&currentCtx->handle,&sem->waitList);
    }
}

void sem_up(struct sem *sem){
    sem->cpt = sem->cpt +1;
    if (sem->cpt <= 0){
        struct ctx_sLinked* newCurrent = list_first_entry(&sem->waitList, struct ctx_sLinked,handle);
        list_del(&newCurrent->handle);
        list_add(&newCurrent->handle,&archives);
    }
}
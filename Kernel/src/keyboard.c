#include "minilib.h"
#include "ioport.h"
#include "semaphore.h"

char azerty_map[256] = {
    0, 0, '&', 0, '"', '\'', '(', '-', 0, '_', 0, 0, ')', '=',
    '\b', '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^',
    '$', '\n', 0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 0,
    0, '\\',0, 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0,
    0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-',
    '4', '5', '6', '+', '1', '2', '3', '0', '.', 0, 0, 0, '='
};

char keyboard_map(unsigned char c ){
    return azerty_map[c];
}

void writeKeyboard(){
    char c = keyboard_map(_inb(0x60));
        if (c != 0){
            putc(c);
        }
}

char getc(){
    char c = keyboard_map(_inb(0x60));
    sem_down(keyboardSem);
    return c;
}

// creer buffer 
// irq setup (1,handle)

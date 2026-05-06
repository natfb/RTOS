#include "./header_files/io_ei.h"

void config_interruption(void) {
    TRISBbits.TRISB0 = 1;             // INT0 como entrada
    INTCON2bits.INTEDG0 = 1;          // Interrupção na borda de subida
    INTCONbits.INT0F = 0;             // Zera a flag de interrupção
    INTCONbits.INT0IE = 1;            // Habilita interrupção INT0
    INTCONbits.GIE = 1;               // Habilita interrupções globais
    INTCONbits.PEIE = 1;              // Habilita interrupções de periféricos
}
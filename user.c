#include "./header_files/user.h"
#include <xc.h>
#include "./header_files/kernel.h"
#include "./header_files/sync.h"
#include "./header_files/com.h"



sem_t s;
pipe_t p;
mutex_t m;


void config_user()
{
    TRISCbits.RC6       = 0;
    TRISCbits.RC7       = 0;
    TRISDbits.RD0       = 0;
    ANSELDbits.ANSD0    = 0;
    ANSELCbits.ANSC6    = 0;
    ANSELCbits.ANSC7    = 0;
    
    asm("global _teste_mutex, _teste_mutex2");

    sem_init(&s, 0);    
    pipe_init(&p);
    mutex_init(&m);
}

#define LED_RED          PORTDbits.RD0
#define LED_GREEN          PORTCbits.RC7
#define LED_YELLOW          PORTCbits.RC6

TASK teste_mutex() {
    while(1) {
        // if (mutex_lock(&m)) {
        mutex_lock(&m); 
        LED_RED = 1;       // Indica que a Tarefa A quer usar o recurso
        LED_GREEN = 1;          // Liga o recurso compartilhado
        
        os_delay(500);      // Simula um trabalho demorado na seção crítica
        
        LED_GREEN = 0;          // Finaliza o uso
        LED_RED = 0;
        // }
        mutex_unlock(&m); 
        os_delay(100); 
    }
}

TASK teste_mutex2() {
    while(1) {
        mutex_lock(&m); 
        LED_YELLOW = 1;       // Indica que a Tarefa A quer usar o recurso
        LED_GREEN = 1;          // Liga o recurso compartilhado
        
        os_delay(500);      // Simula um trabalho demorado na seção crítica
        
        LED_GREEN = 0;          // Finaliza o uso
        LED_YELLOW= 0;
        // }
        mutex_unlock(&m);
        os_delay(100); 
    }
}

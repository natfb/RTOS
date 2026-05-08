#include "./header_files/hw.h"
#include "./header_files/kernel.h"
#include "./header_files/scheduler.h"
#include "./header_files/os_config.h"
#include "./header_files/user.h"

// Quantum do algoritmo Round-Robin
uint8_t rr_quantum = QUANTUM;

void setup_hardware(void)
{
    // Configura��o do timer
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    T0CONbits.T08BIT = 1;   // 8 bits
    T0CONbits.T0CS = 0;     // Instru��o interna
    T0CONbits.PSA = 0;      // Ativa preescaler
    T0CONbits.T0PS = 0b111; // 1:256
    T0CONbits.TMR0ON = 1;   // Ativa timer
    TMR0 = 0;
}

void __interrupt() ISR(void)
{
    // Interrupção externa INT0: cria tarefa one-shot
    if (INTCONbits.INT0IF && INTCONbits.INT0IE)
    {
        INTCONbits.INT0IF = 0;

        os_create_task(6, task_logger, 1);
    }

    // Timer0: base de tempo do RTOS
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;
        
        for (int i = 0; i < r_queue.size; i++) {
            if (r_queue.TASKS[i].task_delay > 0) {
                r_queue.TASKS[i].task_delay--;

                if (r_queue.TASKS[i].task_delay == 0)
                {
                    r_queue.TASKS[i].task_state = READY;
                }
            }
        }
        
        rr_quantum--;

        if (rr_quantum == 0)
        {
            rr_quantum = QUANTUM;

            SAVE_CONTEXT(READY);
            scheduler();
            RESTORE_CONTEXT();
        }
    }
}
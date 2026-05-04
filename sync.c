#include "./header_files/sync.h"
#include "./header_files/kernel.h"
#include "./header_files/scheduler.h"
#include "header_files/os_config.h"

// Fila de aptos
extern ready_queue_t r_queue;


void sem_init(sem_t *sem, uint8_t valor)
{
    sem->contador           = valor;
    sem->pos_input          = 0;
    sem->pos_output         = 0;
}

void sem_wait(sem_t *sem)
{
    DISABLE_ALL_INTERRUPTS();
    
    sem->contador--;
    if (sem->contador < 0) {
        sem->fila[sem->pos_input] = r_queue.pos_task_running;
        sem->pos_input = (sem->pos_input + 1) % MAX_USER_TASKS;
        // Troca de contexto
        SAVE_CONTEXT(WAITING_SEM);
        scheduler();
        RESTORE_CONTEXT();
    }
    
    ENABLE_ALL_INTERRUPTS();
}

void sem_post(sem_t *sem)
{
    DISABLE_ALL_INTERRUPTS();
    
    sem->contador++;
    if (sem->contador <= 0) {
        // Libera o processo bloqueado a mais tempo
        r_queue.TASKS[sem->fila[sem->pos_output]].task_state = READY;
        sem->pos_output = (sem->pos_output + 1) % MAX_USER_TASKS;
    }
    
    ENABLE_ALL_INTERRUPTS();    
}

// MUTEX

void mutex_init(mutex_t *mutex) {
    mutex->value = 0;
    mutex->owner_task_pos = -1;
    mutex->wait_list.count = 0;
    mutex->wait_list.head = 0;
    mutex->wait_list.tail = 0;
    
    for (int i = 0; i < MAX_USER_TASKS; i++) {
        mutex->wait_list.tasks[i] = 0;
    }
}

void mutex_lock(mutex_t *mutex) {
    DISABLE_ALL_INTERRUPTS();

    if (mutex->value == 0) {
        mutex->value = 1;
        mutex->owner_task_pos = r_queue.pos_task_running;
    } else {
        mutex->wait_list.tasks[mutex->wait_list.head] =  r_queue.pos_task_running;
        mutex->wait_list.head = (mutex->wait_list.head + 1) % MAX_USER_TASKS;
        mutex->wait_list.count++;

        r_queue.TASKS[r_queue.pos_task_running].task_state = WAITING;
        ENABLE_ALL_INTERRUPTS();
        os_yield();
    }

    ENABLE_ALL_INTERRUPTS(); 
}

void mutex_unlock(mutex_t *mutex) {
    DISABLE_ALL_INTERRUPTS();

    if (mutex->owner_task_pos != r_queue.pos_task_running) {
        ENABLE_ALL_INTERRUPTS(); 
        return;
    }

    if (mutex->wait_list.count == 0) {
        mutex->value = 0;
        mutex->owner_task_pos = -1;
    } else {
        uint8_t next_task = mutex->wait_list.tasks[mutex->wait_list.tail];
        mutex->wait_list.tail = (mutex->wait_list.tail + 1) % MAX_USER_TASKS;
        mutex->wait_list.count--;

        mutex->owner_task_pos = next_task;
        
        r_queue.TASKS[next_task].task_state = READY;
    }

    ENABLE_ALL_INTERRUPTS(); 
}

// exemple usage

// mutex_t m;

// mutex_init(&m);
// mutex_lock(&m);
// // regiao critica
// mutex_unlock(&m);

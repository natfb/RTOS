#ifndef SYNC_H
#define	SYNC_H

#include <stdint.h>
#include "os_config.h"

// Padr�o POSIX
typedef struct sem {
    int contador;
    uint8_t fila[MAX_USER_TASKS];
    uint8_t pos_input;
    uint8_t pos_output;
} sem_t;


void sem_init(sem_t *sem, uint8_t valor);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);


// MUTEX
typedef struct task_queue {
    uint8_t tasks[MAX_USER_TASKS];
    int head;
    int tail;
    int count;
} task_queue_t;

typedef struct mutex {
    uint8_t        value;                       // 0 = free; 1 = not free
    task_queue_t   wait_list;
    uint8_t        owner_task_pos;
} mutex_t;

void mutex_init(mutex_t *mutex);
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);

#endif	/* SYNC_H */


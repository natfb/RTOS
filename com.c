#include "./header_files/com.h"
#include "./header_files/mem.h"

void pipe_init(pipe_t *p)
{
    p->fila_dados = (char *)SRAMalloc(PIPE_MAX_SIZE);
    //     p->pos_input    = 0;
    //     p->pos_output   = 0;
    //     sem_init(&p->s_input, PIPE_MAX_SIZE); // há PIPE_MAX_SIZE espaços disponíveis para escrita
    //     sem_init(&p->s_output, 0); // inicialmente, não há dados para leitura
    p->pos_input = 0;
    p->pos_output = 0;
    sem_init(&p->s_input, PIPE_MAX_SIZE); // há PIPE_MAX_SIZE espaços disponíveis para escrita
    sem_init(&p->s_output, 0);            // inicialmente, não há dados para leitura
}

void pipe_write(pipe_t *p, char dado)
{
    sem_wait(&p->s_input);                             // tem espaco livre?
    p->fila_dados[p->pos_input] = dado;                // escreve o dado
    p->pos_input = (p->pos_input + 1) % PIPE_MAX_SIZE; // avança a posição de escrita
    sem_post(&p->s_output);                            // sinaliza que há um novo dado para leitura
}

void pipe_read(pipe_t *p, char *dado)
{
    sem_wait(&p->s_output);                              // há dados disponíveis para leitura?
    *dado = p->fila_dados[p->pos_output];                // lê o dado
    p->pos_output = (p->pos_output + 1) % PIPE_MAX_SIZE; // avança a posição de leitura
    sem_post(&p->s_input);                               // sinaliza que há espaço disponível para escrita
}

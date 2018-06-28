#include "threads.h"

void *sieve(void *param)
{
    unsigned id = *((int *)param); /* identificador da thread sieve (posição no vetor de anel da lista circular)*/
    unsigned idComunicacao = id+1; /* comunicação com thread ao lado (referente a posição no vetor de anel da lista circular)*/
    short testePrimo; /* variável para guardar valor de verificação se o número é primo */
    Resposta resultado; /* variável para guardar resposta (caso o número seje primo) */
    Sieve sieve; /* variável auxiliar, utilizada para fazer verificações */
    while (1)
    {


    }
    return 0;
}

void *resultado(void *param)
{
    short continuar = 1; /* flag que indica se Thread continuará a ser executada */
    while (continuar)
    {
        /* Continua no laço até Thread que a chamou uni-lá */
        /* INICIO SEÇÃO CRÍTICA */
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (resposta->dado == 0)
            {
                /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                if(parada == 1){
                    /* Caso a Thread Principal sinalizou que trabalho devem ser parados */
                    continuar = 0; /* atualiza flag para sinalizar que Thread deve ser encerrada */
                    break; /* sai do primeiro loop */
                }
            }
            bloquearResposta(resposta);
                imprimirResposta(resposta);
            desbloquearResposta(resposta);
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
        /* FIM SEÇÃO CRÍTICA */
        pthread_cond_signal(&gResultado);
    }
    return 0;
}
#include "threads.h"

void *principal(void *param) {}

void *siege(void *param) {}

void *resultado(void *param)
{
    Resposta resultado;
    while (1)
    {
        /* Continua no laço até Thread que a chamou uni-lá */
        /* INICIO SEÇÃO CRÍTICA */
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (resposta.resultado != -1)
            {
                /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
            }
            resposta.resultado = -1; /* indica que o resultado já está sendo consumido */
            qntdNumerosCalculados++; /* atualiza quantos números já foram calculados */
            resultado = resposta; /* cria uma váriavel para imprimir o resultado, para liberar a zona crítiza o mais rápido possível */
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
        /* FIM SEÇÃO CRÍTICA */

        if (resultado.ePrimo == 1)
        {
            printf("%d is prime (stored in sieve %d)", resultado.resultado, resultado.siege); /* caso o número for primo */
        }
        else
        {
            printf("%d divided by %d at sieve %d", resultado.resultado, resultado.divisor, resultado.siege); /* caso o número não for primo */
        }
        fflush(stdout);
    }
}
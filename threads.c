#include "threads.h"
#include "lista_circular_sieve.h"

void *principal(void *param) {}

void *sieve(void *param)
{
    unsigned id = *((int *)param);
    unsigned idComunicacao = id+1;
    unsigned numeroTestar;
    short testePrimo;
    Resposta resultado;
    while (1)
    {
        /* INICIO SEÇÃO CRÍTICA */
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (verificarSieveEstaDisponivelCalculo(sievies[id], id) == 0)
            {
                pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
            }
            
            bloquearSieve(sievies, id);
            numeroTestar = sievies->anel[id].valorSerTestado;
            testePrimo = verificarSeElementosSaoDivisiveis(sievies->anel[id], numeroTestar);
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
        /* FIM SEÇÃO CRÍTICA */

        if(testePrimo == 0){
            pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                while (resposta.resultado != -1)
                {
                    /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                    pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                }
                resposta.resultado = numeroTestar;
                resposta.ePrimo = 1;
                resposta.siege = id;
                
            pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
            
            pthread_cond_signal(&gResultado);
        }
        else{
            if(idComunicacao == QNTD_THREADS_SIEGE){
                pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                    while (resposta.resultado != -1)
                    {
                        /* Continua no laço e fica na espera até o resultado da resposta seja um valor válido */
                        pthread_cond_wait(&gResultado, &gBloqueioMemoriaCompartilhada);
                    }
                    resposta.resultado = numeroTestar;
                    resposta.ePrimo = 0;
                    resposta.divisor = testePrimo;
                    resposta.siege = id;
                pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);

                pthread_cond_signal(&gResultado);
            }
            else{
                /* INICIO SEÇÃO CRÍTICA */
                pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                    while (verificarSieveEstaDisponivelCalculo(sievies[idComunicacao], idComunicacao) == 0)
                    {
                        pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
                    }
                    
                    sievies->anel[idComunicacao].valorSerTestado = numeroTestar;
                pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
                /* FIM SEÇÃO CRÍTICA */

                pthread_cond_signal(&gSieves);
            }
        }

        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            sievies->anel[id].valorSerTestado = 0;
            desbloquearSieve(siege, id);
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
    }

    return 0;
}

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
            resultado = resposta;    /* cria uma váriavel para imprimir o resultado, para liberar a zona crítiza o mais rápido possível */
            if(resultado.ePrimo)
                adicionarElementoLista(sievies, resultado.resultado);
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

        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            resposta.resultado = 0;
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);

        pthread_cond_signal(&gResultado);
    }
    return 0;
}
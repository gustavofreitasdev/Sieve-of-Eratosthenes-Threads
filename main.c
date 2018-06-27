#include "fila.c"
#include "threads.c"

int main(int argc, char argv[])
{
    unsigned n = 10;
    unsigned m = 2;
    pthread_t tSieves[QNTD_THREADS_SIEVE];
    pthread_t tResultado;
    short continuar = 1;
    No *buffer = criarFila();   
    unsigned valoresTestar[n];
    unsigned cursorBuffer = 0;
    int aux = 2;
    for(int cursor=0; cursor<n; cursor++){
        valoresTestar[cursor] = aux;
        aux++;
    }
    aux = 0;
    resposta.resultado = -1;
    sievies = criarLista(QNTD_THREADS_SIEVE);


    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        pthread_create(&tSieves[i], NULL, sieve, &i);
    }
    pthread_create(&tResultado, NULL, resultado, NULL);

    while(continuar){
        /* INICIO SEÇÃO CRÍTICA */
        if(buffer->quantidade < m){
            inserirFila(buffer, valoresTestar[cursorBuffer]);
            cursorBuffer++;
        }
        if(buffer->quantidade > 0){
            aux = retirarFila(buffer);
            pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
                while (verificarSieveEstaDisponivelCalculo(sievies, 0) == 0)
                {
                    pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
                }
                
                sievies->anel[0].valorSerTestado = aux;
            pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);

            pthread_cond_signal(&gSieves);
        }
        
    }

    pthread_join(tResultado, NULL);
    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        pthread_join(tSieves[i], NULL);
    }
    return 0;
}
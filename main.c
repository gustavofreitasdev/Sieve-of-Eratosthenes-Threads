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
    unsigned qntdValoresTestados = 0;
    int aux = 2;
    for(int cursor=0; cursor<n; cursor++){
        valoresTestar[cursor] = aux;
        aux++;
    }
    resposta.resultado = -1;
    sievies = criarLista(QNTD_THREADS_SIEVE);


    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        pthread_create(&tSieves[i], NULL, sieve, &i);
    }
    pthread_create(&tResultado, NULL, resultado, NULL);

    while(qntdValoresTestados == n){
        /* INICIO SEÇÃO CRÍTICA */
        if(buffer->quantidade < m){
            inserirFila(buffer, *valoresTestar);
        }
        pthread_mutex_lock(&gBloqueioMemoriaCompartilhada);
            while (verificarSieveEstaDisponivelCalculo(sievies, 0) == 0)
            {
                pthread_cond_wait(&gSieves, &gBloqueioMemoriaCompartilhada);
            }
            
            sievies->anel[0].valorSerTestado = *valoresTestar;
            qntdValoresTestados = qntdNumerosCalculados;
        pthread_mutex_unlock(&gBloqueioMemoriaCompartilhada);
    }

    pthread_join(tResultado, NULL);
    for (int i = 0; i < QNTD_THREADS_SIEVE; i++)
    {
        pthread_join(tSieves[i], NULL);
    }
    return 0;
}
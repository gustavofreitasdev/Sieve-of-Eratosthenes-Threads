#include "lista_circular_sieve.h"

ListaCircularSieve *criarLista(int tamanho)
{
    ListaCircularSieve *resultado = (ListaCircularSieve *)malloc(sizeof(ListaCircularSieve)); /* tenta alocar espaço para lista circular */
    if (resultado)
    {
        /* Caso consiga alocar espaço para lista */
        resultado->tamanhoMaximo = tamanho;                         /* define tamanho da lista */
        resultado->anel = (Sieve *)malloc(sizeof(Sieve) * tamanho); /* tenta alocar espaço para amazenar anel contendo Sieve */
        if (resultado->anel)
        {
            /* Caso consiga alocar espaço para anel */
            for (int cursor = 0; cursor < tamanho; cursor++)
            {
                /* Define e inicializa cada Sieve no anel da Lista Circular */
                Sieve sieve;
                sieve.valoresTestados = (unsigned *)malloc(sizeof(unsigned) * BUFFER_SIEVE); /* tenta alocar lista cotendo valores testados */
                if (sieve.valoresTestados)
                {
                    /* Caso consiga alocar lista contento valores a ser testados */
                    sieve.posicao = cursor; /* posicao que o Sieve está no anel */

                    sieve.valorSerTestado = 0;
                    sieve.qntdValoresTestados = 0;
                    sieve.estado = DESBLOQUEADO;
                    sieve.cursor = 0;
                    resultado->anel[cursor] = sieve;
                }
                else
                {
                    /* Caso não consiga alocar lista para valores a ser testados */
                    printf("[erro] ao alocar espaço para lista de valores a ser testados (sieve)\n");
                    fflush(stdout);
                    free(resultado->anel); /* libera memória alocada para anel */
                    free(resultado);       /* libera memória alocada para o resultado */
                }
            }
        }
        else
        {
            /* Caso não consiga alocar espaço para anel */
            printf("[erro] ao alocar espaço para anel (lista circular sieve)\n");
            fflush(stdout);
            free(resultado); /* libera a memória alocada para o resultado */
        }
    }

    return resultado;
}
void liberarLista(ListaCircularSieve *lista)
{
    free(lista->anel);
    free(lista);
}

void bloquearSieve(ListaCircularSieve *sieves, unsigned posicao)
{
    if (sieves && posicao < sieves->tamanhoMaximo && sieves->anel[posicao].estado == DESBLOQUEADO)
        sieves->anel[posicao].estado = BLOQUEADO;
}
void desbloquearSieve(ListaCircularSieve *sieves, unsigned posicao){
    if (sieves && posicao < sieves->tamanhoMaximo && sieves->anel[posicao].estado == BLOQUEADO)
        sieves->anel[posicao].estado = DESBLOQUEADO;
}
short verificarSieveDisponivel(ListaCircularSieve *sieves, unsigned posicao){
    short disponivel = DESBLOQUEADO;
    if(sieves && posicao < sieves->tamanhoMaximo){
        disponivel = sieves->anel[posicao].estado;        
    }
    return disponivel;
}
short verificarSieveEstaDisponivelCalculo(ListaCircularSieve *sieves, unsigned posicao){
    short disponivel = 0;
    if(sieves && posicao < sieves->tamanhoMaximo){
        if(sieves->anel[posicao].estado == DESBLOQUEADO && sieves->anel[posicao].valorSerTestado != 0){
            disponivel = 1;
        }
    }
    return disponivel;
}
void setValorSerTestado(ListaCircularSieve *sieves, unsigned posicao, unsigned novoValor){
    if(sieves && posicao < sieves->tamanhoMaximo && sieves->anel[posicao].estado == DESBLOQUEADO && sieves->anel[posicao].valorSerTestado == 0){
        sieves->anel[posicao].valorSerTestado = novoValor;
    }
}

short adicionarElementoLista(ListaCircularSieve *sieves, unsigned elemento){
    short elementoAdicionado = 0;
    if(sieves){
        int posicao = sieves->cursor;
        Sieve sieve = sieves->anel[posicao];
        if(sieve.qntdValoresTestados < BUFFER_SIEVE){
            sieves->anel[posicao].valoresTestados[sieve.qntdValoresTestados] = elemento;
            sieves->anel[posicao].qntdValoresTestados++;
            elementoAdicionado = 1;
        }
        if(sieves->cursor+1 < sieves->tamanhoMaximo)
            sieves->cursor++;
        else
            sieves->cursor = 0;
    }

    return elementoAdicionado;
}

void imprimirLista(ListaCircularSieve *sieves)
{
    Sieve sieve;
    unsigned *anel;
    for (int cursor = 0; cursor < sieves->tamanhoMaximo; cursor++)
    {
        sieve = sieves->anel[cursor];
        if (sieve.estado == BLOQUEADO)
            printf("[sieve %d] valor ser testado %d - bloqueado\n", cursor, sieves->anel[cursor].valorSerTestado);
        else
            printf("[sieve %d] valor ser testado %d - desbloqueado\n", cursor, sieves->anel[cursor].valorSerTestado);
        fflush(stdout);


        anel = sieve.valoresTestados;
        for(int cursor2=0; cursor2<sieve.qntdValoresTestados; cursor2++){
            printf("%d ", anel[cursor2]);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
}

short verificarSeElementosSaoDivisiveis(Sieve sieve, unsigned elementoComparado){
    short divisiveis = 0;
    unsigned elemento;
    for(int cursor=0; cursor<sieve.qntdValoresTestados; cursor++){
        elemento = sieve.valoresTestados[cursor];
        if(elementoComparado % elemento == 0 || elemento == 2){
            divisiveis = elemento;
            break;
        }
    }

    return divisiveis;
}

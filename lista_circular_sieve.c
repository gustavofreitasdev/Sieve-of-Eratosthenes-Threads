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
    for(int cursor=0; cursor<lista->tamanhoMaximo; cursor++){
        /* Loop para liberar todo espaço alocado para valores testados de cada Sieve da lista */
        free(lista->anel[cursor].valoresTestados);
    }
    free(lista->anel); /* libera espaço alocado para anel (vetor de sieve) */
    free(lista); /* libera espaçõ alocado para a Lista Circular Sieve */
}
void bloquearSieve(ListaCircularSieve *sieves, unsigned posicao)
{
    if (sieves && posicao < sieves->tamanhoMaximo && sieves->anel[posicao].estado == DESBLOQUEADO) /* Caso Sieve e posição for válida e o Siege estiver desbloqueado */
        sieves->anel[posicao].estado = BLOQUEADO;
}
void desbloquearSieve(ListaCircularSieve *sieves, unsigned posicao)
{
    if (sieves && posicao < sieves->tamanhoMaximo && sieves->anel[posicao].estado == BLOQUEADO) /* Caso Sieve e posição for válida e o Siege estiver bloqueado */
        sieves->anel[posicao].estado = DESBLOQUEADO;
}
void setValorSerTestado(ListaCircularSieve *sieves, unsigned posicao, unsigned novoValor)
{
    if (sieves && posicao < sieves->tamanhoMaximo && sieves->anel[posicao].estado == DESBLOQUEADO && sieves->anel[posicao].valorSerTestado == 0)
    {
        /* Caso o Sieve e a posição sejam válidas, o Sieve estiver desbloqueado e o valor a ser testado for 0 */
        sieves->anel[posicao].valorSerTestado = novoValor; /* muda o valor a ser testado da Sieve */
    }
}
void adicionarElementoLista(ListaCircularSieve *sieves, unsigned elemento)
{
    if (sieves)
    {
        /* Caso a Lista Circular Sieve for válida */
        int posicao = sieves->cursor; /* captura posição que está o cursor */
        Sieve sieve = sieves->anel[posicao]; /* captura a sieve para facilitar as verificações */
        if (sieve.qntdValoresTestados < BUFFER_SIEVE)
        {
            /* Caso o vetor não estiver cheio */
            sieves->anel[posicao].valoresTestados[sieve.qntdValoresTestados] = elemento; /* adiciona o novo elemento no final do vetor */
            sieves->anel[posicao].qntdValoresTestados++; /* incrementa a quantidade de valores testados (adiciona só quando o elemento já foi testado) */
        }
        /* Atualiza o cursor para manter os vetores de valores testados de cada Sieve balanceado */
        if (sieves->cursor + 1 < sieves->tamanhoMaximo) /* caso não estiver no 'último' valor da lista */
            sieves->cursor++;
        else /* caso estiver no 'último' valor da lista */
            sieves->cursor = 0; /* volta pro início */
    }
}
void imprimirLista(ListaCircularSieve *sieves)
{
    Sieve sieve; /* variável para auxilar na impressão da lista */
    unsigned *anel; /* váriavel par auxilar na impressão dos valores testados de cada Sieve */
    for (int cursor = 0; cursor < sieves->tamanhoMaximo; cursor++)
    {
        /* Loop para percorrer todos os Sieves na Lista Circular Sieve */
        sieve = sieves->anel[cursor]; /* captura a sieve atual */
        if (sieve.estado == BLOQUEADO) /* caso a Sieve estiver bloqueada */
            printf("[sieve %d] valor ser testado %d - bloqueado\n", cursor, sieves->anel[cursor].valorSerTestado);
        else /* caso a Sieve estiver desbloqueada */
            printf("[sieve %d] valor ser testado %d - desbloqueado\n", cursor, sieves->anel[cursor].valorSerTestado);
        fflush(stdout);

        anel = sieve.valoresTestados; /* captura vetor com valores testados pela sieve atual */
        for (int cursor2 = 0; cursor2 < sieve.qntdValoresTestados; cursor2++)
        {
            /* Loop para percorrer todos os valores testados pela Sieve atual */
            printf("%d ", anel[cursor2]);
            fflush(stdout);
        }
        printf("\n");
        fflush(stdout);
    }
}

short verificarSieveDisponivel(ListaCircularSieve *sieves, unsigned posicao)
{
    short estado = DESBLOQUEADO; /* considerando inicialmente que a Sieve está desbloqueado */
    if (sieves && posicao < sieves->tamanhoMaximo)
    {
        /* Caso a Sieve e a posição for válida */
        estado = sieves->anel[posicao].estado; /* captura o estado da Sieve */
    }
    return estado;
}
short verificarSieveEstaDisponivelCalculo(ListaCircularSieve *sieves, unsigned posicao)
{
    short estado = 0; /* considerando inicialmente que a Sieve não está disponível para o cálculo */
    if (sieves && posicao < sieves->tamanhoMaximo)
    {
        /* Caso a Sieve e a posição for válida */
        if (sieves->anel[posicao].estado == DESBLOQUEADO && sieves->anel[posicao].valorSerTestado != 0)
        {
            /* Caso a Sieve estiver desbloqueada e o valor a ser testado for diferente de 0 */
            estado = 1; /* muda o estado para disponível para o cálculo */
        }
    }
    return estado;
}
short verificarSeElementosSaoDivisiveis(Sieve sieve, unsigned elementoComparado)
{
    short divisiveis = 0; /* flag que indicara se algum elemento dos valores testados são divisiveis, inicialmente consideramos que não tenha */
    unsigned elemento; /* variável para auxilar nos testes */
    for (int cursor = 0; cursor < sieve.qntdValoresTestados; cursor++)
    {
        /* Loop que percorre o vetor de valores testados da Sieve */
        elemento = sieve.valoresTestados[cursor]; /* captura o elemento a ser comparado atual */
        if (elementoComparado % elemento == 0)
        {
            /* Se o elemento comparado for divisível por 2 */
            divisiveis = elemento; /* captura elemento que conseguiu dividir e para o loop */
            break;
        }
    }

    return divisiveis;
}
short propagarDadoNaRede(ListaCircularSieve *sieves, unsigned dado){
    short dadoEnviado = 0; /* flag responsável guardar se dado foi propagar (1) ou não (0)*/
    if(sieves && dado > 0){
        /* Caso a Lista Circular Sieve e o dado for válidos */
        if(sieves->anel[0].estado == DESBLOQUEADO){
            /* Caso a Sieve estiver desbloqueda */
            sieves->anel[0].valorSerTestado = dado; /* altera valor, que será propagado na rede */
            dadoEnviado = 1; /* indica que dado foi propagado */
        }
    }
    return dadoEnviado;
};

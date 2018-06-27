#include "lista_circular_sieve.c"

int main(void){
    ListaCircularSieve *sieves = criarLista(8);
    bloquearSieve(sieves, 0);
    bloquearSieve(sieves, 3);
    short teste = verificarSieveDisponivel(sieves, 0);
    short teste2 = verificarSieveDisponivel(sieves, 1);
    adicionarElementoLista(sieves, 2);
    adicionarElementoLista(sieves, 3);
    adicionarElementoLista(sieves, 4);
    printf("teste1 (block): %d, teste2(desbloc): %d\n", teste, teste2);


    short teste3 = verificarSeElementosSaoDivisiveis(sieves->anel[0], 4);
    short teste4 = verificarSeElementosSaoDivisiveis(sieves->anel[0], 2);
    printf("teste3 (divi): %d, teste4 (n divi): %d\n", teste3, teste4);


    imprimirLista(sieves);

    desbloquearSieve(sieves, 0);
    setValorSerTestado(sieves, 0, 6);
    setValorSerTestado(sieves, 2, 8);
    setValorSerTestado(sieves, 3, 9);
    imprimirLista(sieves);

    Sieve *sieve = getSieveDisponivelCalculo(sieves);
    printf("Sieve disponivel: %d\n", sieve->posicao);

    return 0;
}
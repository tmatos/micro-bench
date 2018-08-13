
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define N 13   // qtd de coeficientes

//#define VERBOSO

// os coeficientes (fpb)
int h[N] = { 
	-24738871,
	-112681234,
	-170991139,
	-74200673,
	241328526,
	620061218,
	792031499,
	620061218,
	241328526,
	-74200673,
	-170991139,
	-112681234,
	-24738871
};

// os coeficientes (impulso)
int h_imp[N] = { 
	0,
	0,
	0,
	0,
	0,
	0,
    2147483647,
	0,
	0,
	0,
	0,
	0,
	0
};

void filtro_na_cpu( int* entrada, int* saida, int qtde_nums) {

	int buff[N]; // buffer circular para guardar as amostras
    
    int i; // indice do buffer indicando a posicao da ultima leitura

    int j; // indice para iteracao no buffer circular
    int k; // indice para iteracao nos coeficientes

    int older; // posicao no buffer da amostra mais antiga

    int s; // indice para as amostras da entrada
    
    long acc; // acumulador usado para a convolucao
    
    // set bufffer para valor inicial
    for( i = 0 ; i < N ; ++i )
    {
      buff[i] = 0;
    }
    
    i = 0;
    k = 0;

    for ( s = 0 ; s < qtde_nums ; )
    {
      for ( i = 0 ; i < N && s < qtde_nums ; ++i )
      {        
        buff[i] = entrada[s];
        
        k = 0;          
        acc = 0;

        older = (i == N-1) ? 0 : i+1;
        
        for( j = older ; j < N ; ++j , ++k )
        {
          acc += (long)h[k] * (long)buff[j]; 
        }
        
        for( j = 0 ; j < older ; ++j , ++k )
        {
          acc += (long)h[k] * (long)buff[j]; 
        }

        acc = acc + 0x0000000040000000; // para arredondamento

        saida[s] = (int) (acc >> 31); // converte para os 32 bits

        s++;
      }

    }
}

#define QTD_NUMS 128

#define ENTRADA_DAT "entrada.dat"
#define SAIDA_DAT "saida.dat"

#define SAVE_OUTPUT

int main()
{
    FILE* fp_entrada = fopen(ENTRADA_DAT, "r");

    if(!fp_entrada)
    {
        printf("Fornecer o arquivo \"%s\".\n", ENTRADA_DAT);
        exit(1);
    }

    int i;

    int numeros[QTD_NUMS];
    int resultados[QTD_NUMS];

    for(i=0 ; i<QTD_NUMS ; i++)
    {
        if( fscanf(fp_entrada, "%i", numeros+i) != 1 )
        {
            printf("Erro na leitura do arquivo \"%s\".\n", ENTRADA_DAT);
            exit(2);
        }

        resultados[i] = 0;
    }

    fclose(fp_entrada);

  	struct timespec inicio;
  	struct timespec fim;

    clock_gettime(CLOCK_REALTIME, &inicio);  /// T_zero

  	filtro_na_cpu( numeros, resultados, QTD_NUMS );

    clock_gettime(CLOCK_REALTIME, &fim);  /// T_final

    long tempo = ((fim.tv_sec * 1000000000 + fim.tv_nsec) - (inicio.tv_sec * 1000000000 + inicio.tv_nsec));

    #ifdef VERBOSO

    printf("\nTeste da operacao em CPU:\n\n");

    printf("Entrada = ");

    for(i=0 ; i<QTD_NUMS ; i++)
    {
        printf("%i ; ", numeros[i] );
    }

    printf("\n\nSaida = ");

    for(i=0 ; i<QTD_NUMS ; i++)
    {
        printf("%i ; ", resultados[i] );
    }

    struct timespec tp;
    clock_getres(CLOCK_REALTIME, &tp);
    long precisao = (tp.tv_sec * 1000000000 + tp.tv_nsec);

    printf("\n\n");
  	printf("TEMPO GASTO (CPU): %ld (ns)\n", tempo );
    printf("   PRECISAO (CPU): %ld (ns)\n\n", precisao );

    #endif

    #ifndef VERBOSO

  	printf("%ld\n", tempo );

    #endif

    #ifdef SAVE_OUTPUT

    FILE* fp_saida = fopen(SAIDA_DAT, "w");

    if(!fp_saida)
    {
        printf("Nao foi possivel gravar o arquivo \"%s\".\n", SAIDA_DAT);
        exit(10);
    }

    for(i=0 ; i<QTD_NUMS ; i++)
    {
        fprintf(fp_saida, "%i\n", resultados[i] );
    }

    fclose(fp_saida);

    #endif

    exit(0);
}

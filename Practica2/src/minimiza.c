/*
Archivo: minimiza.c
Funcionalidad: Algoritmo y funciones de minimización de un AFD.
Autores: Alfonso Camblor y Julio Carreras
*/

#include "../include/minimiza.h"


/*
Funcion: finalesAFND
Funcionalidad: obtener los estados finales del autómata.
Argumentos:
    AFND* afnd = automata finito determinista creado y listo para ser minimizado.
    int numEstados = numEstados del AFND.
*/
int *finalesAFND(AFND *afnd, int numEstados)
{
    int i, j = 1;

    int *finales = NULL;

    finales = (int *)malloc(sizeof(int));
    finales[0] = 0;
    for (i = 0; i < numEstados; i++)
    {   
        
        if((AFNDTipoEstadoEn(afnd, i) == 1) || (AFNDTipoEstadoEn(afnd, i) == 2)){
            finales[0]++;
            finales = realloc(finales, (finales[0] * sizeof(int)) + sizeof(int));
            finales[j] = i;
            j++;
        }
    }
        
    return finales;
}

int **matriz_finales(AFND *afnd, int numEstados, int* fin)
{   
    int i, j, k, final;
    int **matriz_dist;
    matriz_dist = (int **)malloc(sizeof(int*) * (AFNDNumEstados(afnd)-1));
    if (!matriz_dist)
        return NULL;

    for(i = 0;i<AFNDNumEstados(afnd)-1;i++){
        matriz_dist[i] = (int *)malloc(sizeof(int) * (i+2));

        printf("Estado añadido %d\n", i+1);


        matriz_dist[i][0] = i+1;
        printf("%d\n\n", matriz_dist[0][0]);
    }


    
    matriz_dist[i] = (int *)malloc(sizeof(int) * (i+1));

    for(j = 0;j<AFNDNumEstados(afnd)-1;j++){
        matriz_dist[i][j+1] = j;
    }

    for(i = 0;i<AFNDNumEstados(afnd);i++){
        for(j = 0;j<=i+1;j++){
            printf("%d\t", matriz_dist[i][j]);
        }
        printf("\n");
    }


    printf(" Número de estados finales: %d\n", fin[0]);
    for (i = 1;i<=fin[0]; i++){
        printf("q = %d \n", fin[i]); 

        final = fin[i];
    

        /* Vertical */
        for(j = final; j<(AFNDNumEstados(afnd)-1); j++){
            /*printf("Se accede a matriz_dist[%d][%d]\n", j, final);*/
            matriz_dist[j][final+1] = 1;
        }

        /* Horizontal */
        for(k = 1;k <= final;k++){
            matriz_dist[final][k] = 1;
        }
        
    }

    for(i = 0;i<AFNDNumEstados(afnd);i++){
        for(j = 0;j<=i+1;j++){
            printf("%d\t", matriz_dist[i][j]);
        }
        printf("\n");
    }

}


AFND * AFNDMinimiza(AFND * afnd){
    int *fin;
    int **matriz_dist;

    fin = finalesAFND(afnd, AFNDNumEstados(afnd));

    matriz_dist = matriz_finales(afnd, AFNDNumEstados(afnd), fin);

    printf("\n");

    free(fin);
    /*for(i =0;i<AFNDNumEstados(afnd)-1;i++){
        free(matriz_dist[i]);
    }*/

    return afnd;
}

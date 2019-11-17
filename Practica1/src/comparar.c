#include <stdio.h>
#include "../include/comparar.h"
#include "../include/transforma.h"


void imprimeVectorFila2(int *vector, int fila)
{
    int i;
    printf("Vector fila %d: {", fila);
    for (i = 1; i <= vector[0]; i++)
    {
        printf(" %d ", vector[i]);
    }
    printf("}\n");
}

int compararVectores(int* vector1, int*vector2)
{
    int i, j;
    int rec;
    int indice;
    int count1 = 0;
    int count2 = 0;

    if(vector1[0] != vector2[0]){
        return vector1[0] - vector2[0];
    }

    for(indice = 1; indice<=vector1[0];indice++){
        count1+= vector1[indice];
    }
    for(indice = 1; indice<=vector2[0];indice++){
        count2+= vector2[indice];
    }
    
    if(count1 == count2){
        return 0;
    } else {
        for(i=1, rec = 0;i<=vector1[0];i++){
            for(j=1;j<=vector2[0];j++)
            {
                if(vector1[i] == vector2[j]){
                    rec++;
                    break;                }
            }

        }
        if(rec == vector1[0]){
            return 0;
        }
        return vector1[0]-rec;
    }
}
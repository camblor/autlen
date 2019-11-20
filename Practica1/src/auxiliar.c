#include <stdio.h>
#include "../include/auxiliar.h"

/*---------------
 Funciones MergeSort
---------------*/

void merge(int *arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int *arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

/*
 Funcion de comparacion entre vectores. 
 */

int compararVectores(int *vector1, int *vector2)
{
    int i, j;
    int rec;
    int indice;
    int count1 = 0;
    int count2 = 0;

    if (vector1[0] != vector2[0])
    {
        return vector1[0] - vector2[0];
    }

    for (indice = 1; indice <= vector1[0]; indice++)
    {
        count1 += vector1[indice];
    }
    for (indice = 1; indice <= vector2[0]; indice++)
    {
        count2 += vector2[indice];
    }

    if (count1 == count2)
    {
        return 0;
    }
    else
    {
        for (i = 1, rec = 0; i <= vector1[0]; i++)
        {
            for (j = 1; j <= vector2[0]; j++)
            {
                if (vector1[i] == vector2[j])
                {
                    rec++;
                    break;
                }
            }
        }
        if (rec == vector1[0])
        {
            return 0;
        }
        return vector1[0] - rec;
    }
}

/*
Funcion: estudiarAFND
Funcionalidad: cubo de transiciones del automata finito no determinista.
Argumentos:
    AFND* afnd = automata finito no determinista creado y listo para ser transformado.
    int numEstados = numEstados del AFND.
    int numSimbolos = numSimbolos.
*/
int ***estudiarAFND(AFND *afnd, int numEstados, int numSimbolos)
{
    int i, j, k;
    int ***transiciones = NULL;

    transiciones = (int ***)malloc(sizeof(int **) * numEstados);

    for (i = 0; i < numEstados; i++)
    {
        transiciones[i] = (int **)malloc(sizeof(int *) * (numSimbolos + 1));

        /*Columna: Simbolos*/
        for (j = 0; j <= numSimbolos; j++)
        {
            /*Inicializamos como si no hubiera ninguna transicion*/
            transiciones[i][j] = (int *)malloc(sizeof(int));
            transiciones[i][j][INDEX] = 0;
            for (k = 0; k < numEstados; k++)
            {

                /*Se guardan las transiciones por simbolos de cada estado en la tabla*/
                if (j < numSimbolos && AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, k))
                {
                    transiciones[i][j] = realloc(transiciones[i][j], sizeof(transiciones[i][j]) + sizeof(int));
                    transiciones[i][j][INDEX]++;
                    transiciones[i][j][transiciones[i][j][INDEX]] = k;
                }
                /*Se guardan las transiciones lambda de cada estado en la tabla*/
                else if (j == numSimbolos && i != k && AFNDCierreLTransicionIJ(afnd, i, k))
                {

                    transiciones[i][j][INDEX]++;
                    transiciones[i][j] = realloc(transiciones[i][j], (transiciones[i][j][INDEX] * sizeof(int)) + sizeof(int));
                    transiciones[i][j][transiciones[i][j][INDEX]] = k;
                }
            }
            if (transiciones[i][j][INDEX] == 0)
                transiciones[i][j][INDEX] = -1;
        }
    }
    return transiciones;
}

/*
Funcion: imprimirAFND
Funcionalidad: Imprime la tabla del automata finito no determinista.
Argumentos:
    int*** transiciones = tabla de transiciones del AFND.
    int numEstados = numEstados del AFND.
    int numSimbolos = numSimbolos del AFND.

*/
void imprimirAFND(int ***transiciones, int numEstados, int numSimbolos)
{
    int i, j;
    printf("     +  0  .  l\n");
    printf("   _____________\n");
    for (i = 0; i < numEstados; i++)
    {
        printf("q%d| ", i);
        for (j = 0; j <= numSimbolos; j++)
        {
            printf("%2d ", transiciones[i][j][INDEX]);
        }
        printf("|\n");
    }
    printf("\n");
}

/*
Funcion: contiene
Funcionalidad: Comprueba si el vector tiene el estado almacenado.
Argumentos:
    int* vector = vector donde se comprobara.
    int estado = estado a comprobar su aparicion en el vector.
*/
int contiene(int *vector, int estado)
{
    int i;
    for (i = 1; i <= vector[0]; i++)
    {
        if (vector[i] == estado)
        {
            return 1;
        }
    }
    return 0;
}

/*
Funcion: imprimeVectorFila
Funcionalidad: Imprime por pantalla el vector con un identificador llamado fila por su aplicacion en el debugging
Argumentos:
    int* vector = vector a imprimir.
    int fila = identificador de impresion.
*/
void imprimeVectorFila(int *vector, int fila)
{
    int i;
    printf("Vector fila %d: {", fila);
    for (i = 1; i <= vector[INDEX]; i++)
    {
        printf(" %d ", vector[i]);
    }
    printf("}\n");
}

/*
Funcion: anadirTransiciones
Funcionalidad: Introduce en el vector un nuevo estado del AFD
Argumentos:
    int* vector = Vector a introducir en la tabla de transiciones.
    int indiceEstado = Indice del estado a introducir en el vector.
    int numSimbolos = numSimbolos.
    int*** transiciones = tabla de transiciones del AFND.
*/
int *anadirTransiciones(int *vector, int indiceEstado, int numSimbolos, int ***transicionesT)
{
    int j = 0;
    int flag = 0;

    /* Buscamos si ya existe */
    flag = contiene(vector, indiceEstado);
    /* Se añade el indice */
    if (flag != 1)
    {

        vector[INDEX]++;
        vector = (int *)realloc(vector, (vector[INDEX] * sizeof(int)) + sizeof(int));
        vector[vector[INDEX]] = indiceEstado;
    }
    /* Ese indice puede llevar a otros por lambdas. Se comprueban. */
    for (j = 1; j <= transicionesT[indiceEstado][numSimbolos][INDEX]; j++)
    {

        if (!contiene(vector, transicionesT[indiceEstado][numSimbolos][j]))
        {
            /*printf("AÑADIDALAMBDA a %d: contiene=%d\n", transicionesT[indiceEstado][numSimbolos][j], contiene(vector, transicionesT[indiceEstado][numSimbolos][j]));*/
            vector = anadirTransiciones(vector, transicionesT[indiceEstado][numSimbolos][j], numSimbolos, transicionesT);
        }
    }

    return vector;
}


/*
Funcion: imprimeAFD
Funcionalidad: imprime el AFND tal y como se recibe en la estructura intermedia 2 (tabla de transiciones del AFD)
Argumentos:
    int transicionesDet = tabla de transiciones del AFD.
    AFND *afnd = AFND Original.
    int fila = numero de filas del AFD.
    int numSimbolos = numero de simbolos del AFND Original.
*/
void imprimeAFD(int ***transicionesDet, AFND *afnd, int fila, int numSimbolos)
{
    int i;
    int j;
    int k;

    for (i = 0; i < fila; i++)
    {
        if (transicionesDet[i] == NULL)
        {
            printf("AYUDA EN %d", i);
            return;
        }
        printf("fila %d\n", i);

        for (j = 0; j < numSimbolos; j++)
        {
            if (transicionesDet[i][j] == NULL)
            {
                printf("AYUDA EN %d-%d\n", i, j);
                return;
            }
            printf("Simbolo %s: {", AFNDSimboloEn(afnd, j));
            for (k = 1; k <= transicionesDet[i][j][INDEX]; k++)
            {
                printf(" %d ", transicionesDet[i][j][k]);
            }
            printf("}\n");
        }
        printf("\n");
    }
}

/*
Funcion: addVisitado
Funcionalidad: Introduce un nuevo vector en la tabla de visitados
Argumentos:
    int* vector = vector a introducir.
*/
int **addVisitado(int *vector, int *n_visitados, int **visitados)
{

    (*n_visitados)++;

    visitados[INDEX][INDEX] = (*n_visitados);

    visitados = (int **)realloc(visitados, (visitados[INDEX][INDEX] * sizeof(int *) + sizeof(int *)));
    visitados[visitados[INDEX][INDEX]] = vector;

    return visitados;
}

/*
Funcion: imprimeVisitados
Funcionalidad: Imprime la tabla de los estados visitados.
ArgumentoS:
    tabla de estados visitados.
*/
void imprimeVisitados(int **visitados, int n_visitados)
{
    int i, j;
    printf("VISITADOS:\n");
    visitados[INDEX][INDEX] = n_visitados;
    for (i = 1; i <= visitados[INDEX][INDEX]; i++)
    {
        if (visitados[i] == NULL)
        {
            return;
        }
        printf("{");
        fflush(stdout);

        for (j = 1; j <= visitados[i][INDEX]; j++)
        {
            printf(" %d ", visitados[i][j]);
            fflush(stdout);
        }
        printf("}\n");
        fflush(stdout);
    }
    printf("\n");
}

/*
Funcion: crearVisitados
Funcionalidad: Creacion de la tabla de visitados.
Argumentos:
    int* vector = vector a introducir como primero visitado.
*/
int **crearVisitados(int *vector)
{
    /* 
    Ya tenemos guardadas en la tabla las transiciones del determinista 
    Ahora recorremos esa tabla hasta encontrar un estado que no haya sido procesado.
    Contamos por estados también los conjuntos de estado (Definicion recursiva)
   
    visitados[0][0] = NUMERO DE ESTADOS VISITADOS
    visitados[i] = ESTADO VISITADO QUE PUEDE ESTAR FORMADO POR VARIOS
    visitados[i][0] = NUMERO DE ELEMENTOS DEL ESTADO
    visitados[i][j] = ELEMENTOS DEL ESTADO 
    */
    int **visitados_i;
    visitados_i = (int **)malloc(sizeof(int *) * 4);
    visitados_i[INDEX] = (int *)malloc(sizeof(int));
    visitados_i[INDEX][INDEX] = 1;
    visitados_i[visitados_i[INDEX][INDEX]] = vector;

    return visitados_i;
}

/*
Funcion: inicialDeterminista
Funcionalidad: Crea el cubo del AFD con el estado inicial ya visitado y procesadas las transiciones lambda.
Argumentos:
    int* vector = vector de la primera fila de la tabla de transiciones del AFD.
    int numSimbolos = numSimbolos.
    int*** transiciones = tabla de transiciones del AFND.
    int fila = indice de la fila en la tabla del AFD.
*/
int ***inicialDeterminista(int *vector, int numSimbolos, int ***transiciones, int fila)
{
    int i, j, k;
    int n;

    /*
    Procesamos estados accesibles desde los estados iniciales

    Tenemos almacenados los estados iniciales en inicialDefinitivo
    Por cada estado en inicialDefinitivo, consultamos sus transiciones con cada simbolo
    y las guardamos en transicionesDet[0][j].

    transicionesDet[0][j][0] = numero de transiciones de esa celda
    transicionesDet[0][j][1] = transicion 1
    transicionesDet[0][j][2] = transicion 2
    ...
    transicionesDet[0][j][n] = transicion n
   */
    int ***transicionesDet;

    transicionesDet = (int ***)malloc(sizeof(int **));
    if (transicionesDet == NULL)
    {
        return NULL;
    }

    transicionesDet[fila] = (int **)malloc(sizeof(int *) * (numSimbolos + 1));
    if (transicionesDet[fila] == NULL)
    {
        return NULL;
    }

    for (k = 0; k < numSimbolos; k++)
    {
        transicionesDet[fila][k] = (int *)malloc(sizeof(int));
        if (transicionesDet[fila][k] == NULL)
        {
            return NULL;
        }
        transicionesDet[fila][k][INDEX] = 0;
    }

    for (i = 1; i <= vector[INDEX]; i++)
    {
        /* Para cada estado inicial del Determinista */
        for (j = 0; j < numSimbolos; j++)
        {
            /*Consultamos todos los simbolos*/
            /*Inicializamos poniendo a 0 el numero de cada transicion*/

            /* Si hay alguna transicion desde estado inicial actual con simbolo j*/
            if (transiciones[vector[i]][j][INDEX] != -1)
            {
                /*Reservamos la memoria necesaria para las transiciones que haya*/
                if (transicionesDet[fila][j] == NULL)
                {
                    return NULL;
                }

                /*Y las guardamos en la tabla del determinista*/
                for (k = 1; k <= transiciones[vector[i]][j][INDEX]; k++)
                {
                    printf("Con el simbolo %d:\n", j);
                    imprimeVectorFila(transicionesDet[fila][j], fila);
                    if (!contiene(transicionesDet[fila][j], transiciones[vector[i]][j][k]))
                    {
                        transicionesDet[fila][j][INDEX]++;
                        transicionesDet[fila][j] = realloc(transicionesDet[fila][j], (transicionesDet[fila][j][INDEX] * sizeof(int)) + sizeof(int));
                        transicionesDet[fila][j][transicionesDet[fila][j][INDEX]] = transiciones[vector[i]][j][k];
                        printf("SE HA AÑADIDO AL INICIAL %d\n", transiciones[vector[i]][j][k]);
                        if (contiene(transicionesDet[fila][j], transiciones[vector[i]][j][k]))
                        {
                            printf("YA LO TIENE\n");
                            for (n = 1; n <= transiciones[transiciones[vector[i]][j][k]][numSimbolos][INDEX]; n++)
                            {
                                if (!contiene(transicionesDet[fila][j], transiciones[transiciones[vector[i]][j][k]][numSimbolos][n]))
                                {
                                    transicionesDet[fila][j][INDEX]++;
                                    transicionesDet[fila][j] = realloc(transicionesDet[fila][j], (transicionesDet[fila][j][INDEX] * sizeof(int)) + sizeof(int));
                                    transicionesDet[fila][j][transicionesDet[fila][j][INDEX]] = transiciones[transiciones[vector[i]][j][k]][numSimbolos][n];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return transicionesDet;
}

/*
function: obtenerInicial
return: array de enteros con los estados iniciales, lambdas ya tenidas en cuenta
*/
int *obtenerInicial(int ***transiciones, int indiceEstado, int numSimbolos, int *iniciado)
{
    /*
    Guardamos estados iniciales aplicadas lambdas:
    vector[0] = numero de estados iniciales.
    vector[1] = estado inicial 1
    vector[2] = estado inicial 2
    ...
    vector[n] = estado inicial n
    */
    int *vector = NULL;
    vector = (int *)malloc(sizeof(int));
    if (vector == NULL)
    {
        return NULL;
    }
    vector[INDEX] = 1;
    vector = realloc(vector, sizeof(vector) + sizeof(int));
    vector[vector[INDEX]] = indiceEstado;

    if (iniciado == 0)
    {
        int i;
        if (transiciones[indiceEstado][numSimbolos][INDEX] > 0)
        {
            vector = realloc(vector, sizeof(vector) + (sizeof(int) * transiciones[indiceEstado][numSimbolos][INDEX]));

            /*Transiciones lambda*/
            for (i = 1; i <= transiciones[indiceEstado][numSimbolos][INDEX]; i++)
            {

                vector[INDEX]++;
                vector = (int *)realloc(vector, (vector[INDEX] * sizeof(int)) + sizeof(int));
                vector[vector[INDEX]] = transiciones[indiceEstado][numSimbolos][i];
            }
        }
        iniciado++;
    }

    return vector;
}
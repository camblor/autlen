#include "../include/transforma.h"

#define INDEX 0

/* Tabla dinámica para el AFD*/
int ***transicionesDet = NULL;
/* Tabla que almacena los estados visitados */
int **visitados = NULL;
int n_visitados = 0;
AFND *afnd_g = NULL;

/*
function: estudiarAFND
return: cubo de transiciones del automata finito no determinista.
*/
int ***estudiarAFND(AFND *afnd, int numEstados, int numSimbolos)
{
    int i, j, k;
    int ***transiciones = NULL;

    transiciones = (int ***)malloc(sizeof(int **) * numEstados);

    for (i = 0; i < numEstados; i++)
    {
        transiciones[i] = (int **)malloc(sizeof(int *) * numSimbolos + 1);

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

                    transiciones[i][j] = realloc(transiciones[i][j], sizeof(transiciones[i][j]) + sizeof(int));
                    transiciones[i][j][INDEX]++;
                    transiciones[i][j][transiciones[i][j][INDEX]] = k;
                }
            }
            if (transiciones[i][j][INDEX] == 0)
                transiciones[i][j][INDEX] = -1;
        }
    }
    return transiciones;
}

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
function: obtenerInicial
return: array de enteros con los estados iniciales, lambdas ya tenidas en cuenta
*/
int *obtenerInicial(int ***transiciones, int indiceEstado, int numSimbolos)
{
    int i;
    int *inicialDefinitivo = NULL;
    inicialDefinitivo = (int *)malloc(sizeof(int));
    if (inicialDefinitivo == NULL)
    {
        return NULL;
    }
    inicialDefinitivo[INDEX] = 1;
    inicialDefinitivo[inicialDefinitivo[INDEX]] = indiceEstado;

    /*
    Guardamos estados iniciales aplicadas lambdas:
    inicialDefinitivo[0] = numero de estados iniciales.
    inicialDefinitivo[1] = estado inicial 1
    inicialDefinitivo[2] = estado inicial 2
    ...
    inicialDefinitivo[n] = estado inicial n
    */
    if (transiciones[indiceEstado][numSimbolos][INDEX] != 0)
    {
        inicialDefinitivo = realloc(inicialDefinitivo, sizeof(inicialDefinitivo) + sizeof(int) * transiciones[indiceEstado][numSimbolos][INDEX]);
        /*Transiciones lambda*/
        for (i = 0; i < transiciones[indiceEstado][numSimbolos][INDEX]; i++)
        {
            inicialDefinitivo[INDEX]++;
            inicialDefinitivo[inicialDefinitivo[INDEX]] = transiciones[indiceEstado][numSimbolos][i];
        }
    } /*
    
    printf("[");
    for(i=0; i<inicialDefinitivo[INDEX];i++){
        printf(" %d ", inicialDefinitivo[i]);
    }
    printf("]\n");
    */
    return inicialDefinitivo;
}

/*
function: inicialDeterminista
retorno: cubo del AFD con el estado inicial visitado e incluido en transicionesDet[n_estado][n_simbolo].
*/
int ***inicialDeterminista(int *vector, int numSimbolos, int ***transiciones, int fila)
{
    int i, j, k;
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

    for (k = 0; k <= numSimbolos; k++)
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
                transicionesDet[fila][j] = realloc(transicionesDet[fila][j], sizeof(transicionesDet[fila][j]) + sizeof(int) * transiciones[vector[i]][j][INDEX]);
                if (transicionesDet[fila][j] == NULL)
                {
                    return NULL;
                }
                /*Y las guardamos en la tabla del determinista*/
                for (k = 1; k <= transiciones[vector[i]][j][INDEX]; k++)
                {
                    transicionesDet[fila][j][INDEX]++;
                    transicionesDet[fila][j][transicionesDet[fila][j][INDEX]] = transiciones[vector[i]][j][k];

                    /*
                    printf("transicionesDet[%d][%d][0] = %d\n", fila, j , transicionesDet[fila][j][INDEX]);
                    printf("transicionesDet[%d][%d][%d] = %d\n", fila, j , k, transicionesDet[fila][j][transicionesDet[fila][j][INDEX]]);
                    */
                }
            }
        }
    }

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
    return transicionesDet;
}
/*
funcion: crearVisitados
retorno: tabla de visitados
*/
int **crearVisitados(int *vector)
{
    /* 
    Ya tenemos guardadas en la tabla las transiciones del determinista 
    Ahora recorremos esa tabla hasta encontrar un estado que no haya sido procesado.
    Contamos por estados también los conjuntos de estado (Definicion recursiva)
    */
    /*
    visitados[0][0] = NUMERO DE ESTADOS VISITADOS
    visitados[i] = ESTADO VISITADO QUE PUEDE ESTAR FORMADO POR VARIOS
    visitados[i][0] = NUMERO DE ELEMENTOS DEL ESTADO
    visitados[i][j] = ELEMENTOS DEL ESTADO   
    
    transicionesDet[fila][estado][0] = n transiciones
    transicionesDet[fila][estado][x] = transicion
    */
    /*int i, j;*/
    int **visitados_i;
    visitados_i = (int **)malloc(sizeof(int *) * 2);
    visitados_i[INDEX] = (int *)malloc(sizeof(int));
    visitados_i[INDEX][INDEX] = 1;
    visitados_i[visitados_i[INDEX][INDEX]] = vector;
    /*
    printf("VISITADOS:\n");
    for (i = 1; i <= visitados[0][INDEX]; i++)
    {
        printf("{");
        for (j = 1; j <= visitados[i][INDEX]; j++)
        {
            printf(" %d ", visitados[i][j]);
        }
        printf("}\n");
    }
    printf("\n");
    */

    return visitados_i;
}

void imprimeVisitados(int **visitados_i)
{
    int i, j;
    printf("VISITADOS:\n");
    printf("visitados_index = %d\n", visitados[INDEX][INDEX]);
    visitados[INDEX][INDEX] = n_visitados;
    for (i = 1; i <= visitados[INDEX][INDEX]; i++)
    {
        printf("i=%d/%d\n", i, visitados[INDEX][INDEX]);
        if (visitados[i] == NULL)
        {
            printf("aqui\n");
            return;
        }
        printf("{");
        for (j = 1; j <= visitados[i][INDEX]; j++)
        {
            printf(" %d ", visitados[i][j]);
        }
        printf("}\n");
    }
    printf("\n");
}

void addVisitado(int *vector)
{
    visitados = realloc(visitados, sizeof(visitados) + sizeof(int *));
    printf("visitados_index = %d\n", visitados[INDEX][INDEX]);
    visitados[INDEX][INDEX]++;
    printf("visitados_index = %d\n", visitados[INDEX][INDEX]);
    visitados[visitados[INDEX][INDEX]] = vector;
    imprimeVisitados(visitados);
}

int ***nuevaFilaDeterminista(int *vector, int numSimbolos, int ***transiciones, int fila)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    int n = 0;
    int flag = 0;

    /* Memoria */
    if (fila < 0)
    {
        return transicionesDet;
    }

    transicionesDet = (int ***)realloc(transicionesDet, (fila + 1) * sizeof(int **));
    if (transicionesDet[fila] == NULL)
    {
        transicionesDet[fila] = (int **)malloc(sizeof(int *) * (numSimbolos + 1));
    }
    if (transicionesDet[fila] == NULL)
    {
        return NULL;
    }

    for (n = 0; n < numSimbolos; n++)
    {

        if (transicionesDet[fila][n] == NULL)
        {
            transicionesDet[fila][n] = malloc(sizeof(int));
        }

        if (transicionesDet[fila][n] == NULL)
        {
            return NULL;
        }
        transicionesDet[fila][n][INDEX] = 0;
    }

    /* Ver entrada */
    printf("Vector fila %d: {", fila);
    for (i = 1; i <= vector[INDEX]; i++)
    {

        printf(" %d ", vector[i]);
    }
    printf("}\n");

    for (i = 1; i <= vector[INDEX]; i++)
    {
        for (j = 0; j < numSimbolos; j++)
        {
            /*printf("transiciones[%d][%d][%d] = %d\n", vector[i], j, INDEX, transiciones[vector[i]][j][INDEX]);*/
            for (k = 1; k <= transiciones[vector[i]][j][INDEX]; k++)
            {
                /*Recorrer los estados de transicionesDet[fila][j] BUSCANDO SI YA ESTA*/
                for (l = 1, flag = 0; l <= transicionesDet[fila][j][INDEX]; l++)
                {
                    /*Si ya está lo marcamos*/
                    if (transiciones[vector[i]][j][k] == transicionesDet[fila][j][l])
                    {
                        flag = 1;
                    }
                }

                if (flag != 1 && transiciones[vector[i]][j][INDEX] != -1)
                {
                    /*
                    printf("++++++++++++++++++++++++++++++++++++++++++++++++++\n");
                    printf("transicionesDet[%d][%d][%d]++\n", fila, j, INDEX);
                    printf("transicionesDet[%d][%d][%d] = %d\n", fila, j, transicionesDet[fila][j][INDEX], transiciones[vector[i]][j][k]);
                    printf("++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
                    */

                    transicionesDet[fila][j][INDEX]++;

                    transicionesDet[fila][j][transicionesDet[fila][j][INDEX]] = transiciones[vector[i]][j][k];

                    /* Comprobar lambdas para el estado añadido transiciones[vector[i]][j][k]. */
                    for (m = 1; m <= transiciones[transiciones[vector[i]][j][k]][numSimbolos][INDEX]; m++)
                    {

                        transicionesDet[fila][j][INDEX]++;
                        transicionesDet[fila][j][transicionesDet[fila][j][INDEX]] = transiciones[transiciones[vector[i]][j][k]][numSimbolos][m];
                    }
                }
            }
        }
    }
    if (fila == 2)
        printf("\n__\n");

    return transicionesDet;
}

int *anadirTransiciones(int *vector, int ***transiciones, int indiceEstado, int numSimbolos)
{
    int i = 0;
    int flag = 0;

    for (i = 1; i <= vector[INDEX]; i++)
    {
        if (vector[i] == indiceEstado)
        {
            flag = 1;
        }
    }
    if (flag != 1)
    {
        vector = (int *)realloc(vector, sizeof(vector) + sizeof(int));
        vector[INDEX]++;
        vector[vector[INDEX]] = indiceEstado;
    }

    return vector;
}

AFND *AFNDTransforma(AFND *afnd)
{

    /*
    ----------------------------------
            Variables
    ----------------------------------
    */
    /* Proposito general (bucles) */
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    /* Datos generales del AFND */
    int numSimbolos = AFNDNumSimbolos(afnd);
    int numEstados = AFNDNumEstados(afnd);
    afnd_g = afnd;
    /* Tablas dinámicas*/
    int ***transiciones = NULL;
    int *inicialDefinitivo = NULL;

    /* Bandera que indica si el estado ya ha sido visitado */
    int flag = 0;
    int fila = 0;
    int *temporal = NULL;

    /*
    ----------------------------------
            ESTUDIO DEL AFND
    ----------------------------------
    */

    /* Metemos la tabla de transiciones del AFND en transiciones. */
    transiciones = estudiarAFND(afnd, numEstados, numSimbolos);

    /* Ahora debemos rellenar la tabla del AFD. */

    /*
    ----------------------------------
            E.Inicial del AFD
    ----------------------------------
    */
    /* Obtenemos el estado inicial teniendo en cuenta lambdas. */
    inicialDefinitivo = obtenerInicial(transiciones, AFNDIndiceEstadoInicial(afnd), numSimbolos);

    /*
    ----------------------------------
            CREACION TABLA DEL AFD
    ----------------------------------
    */

    /* Metemos la tabla de transiciones del AFND en transiciones. */
    transicionesDet = inicialDeterminista(inicialDefinitivo, numSimbolos, transiciones, 0);
    fila++;
    n_visitados++;
    /* Procesado el estado inicial del AFD. */

    /* Creamos tabla de conjuntos de estados visitados */
    visitados = crearVisitados(inicialDefinitivo);
    /* Ahora que ya esta creado anadimos a ella los estados que vamos a ir visitando en el bucle principal*/
    int test = visitados[INDEX][INDEX];

    for (i = 0; i < test; i++)
    {
        /* Para cada estado i */
        /*printf("FILA %d:\n", i + 1);*/
        for (j = 0; j < numSimbolos; j++)
        {
            /* Introducimos el simbolo j */
            /*printf("SIMBOLO: %s - TRANSICIONES: ", AFNDSimboloEn(afnd, j));*/

            /* y comprobamos los estados ya visitados k */
            for (k = 1; k <= visitados[INDEX][INDEX]; k++)
            {

                flag = 0;
                /* Si tienen el mismo numero de elementos*/
                if (transicionesDet[i][j][INDEX] == visitados[k][INDEX])
                {
                    /*Analizamos elemento a elemento*/
                    for (m = 1; m <= visitados[k][INDEX]; m++)
                    {
                        /*Si algun elemento distinto --> NO ES EL MISMO -> SIGUIENTE (break)*/
                        if (transicionesDet[i][j][m] != visitados[k][m])
                        {
                            flag = 0;
                            break;
                        }
                        /* Si todos los elementos son iguales --> VISITADO flag = 1*/
                        else if (m == visitados[k][INDEX])
                        {
                            flag = 1;
                        }
                    }
                }

                /* Si visitado --> pasamos al siguiente */
                if (flag == 1)
                {
                    break;
                }
                /* Si no visitado --> visitamos y lo metemos a la tabla de visitados*/
                if (k == visitados[0][INDEX] && flag == 0)
                {
                    /*printf("TOCARIA RECORRER -> %d\n", transicionesDet[i][j][INDEX]);*/
                    /* Para cada simbolo l */

                    /* Para cada estado dentro de los conjuntos de estados */
                    temporal = NULL;
                    for (l = 1; l <= transicionesDet[i][j][INDEX]; l++)
                    {
                        if (l == 1)
                        {
                            temporal = obtenerInicial(transiciones, transicionesDet[i][j][l], numSimbolos);
                        }
                        temporal = anadirTransiciones(temporal, transiciones, transicionesDet[i][j][l], numSimbolos);
                        /*TODO actualizar para almacenarlo*/
                        /*addVisitado(temporal);*/
                        if (l == transicionesDet[i][j][INDEX])
                        {
                            transicionesDet = nuevaFilaDeterminista(temporal, numSimbolos, transiciones, fila);
                            if (!transicionesDet)
                            {
                                printf("chao");
                                fflush(stdout);
                                return NULL;
                            }
                            fila++;
                            printf("visitados_index = %d\n", visitados[INDEX][INDEX]);
                            printf("n_visitados = %d\n", n_visitados);
                            n_visitados++;
                            visitados[INDEX][INDEX]++;
                            /*
                            addVisitado(temporal);
                            visitados[visitados[INDEX][INDEX]] = (int**) malloc(sizeof(int*))
                            */
                            visitados[visitados[INDEX][INDEX]] = temporal;
                        }
                    }

                    /*addVisitado(transicionesDet[i][j]);*/

                    /*Acaba de anadirlo*/
                }
            }
        }
    }
    imprimirAFND(transiciones, numEstados, numSimbolos);

    for (i = 0; i < fila; i++)
    {
        if (transicionesDet[i] == NULL)
        {
            printf("es la i=%d\n", i);
            return NULL;
        }
        printf("%s ", AFNDNombreEstadoEn(afnd, i));

        for (j = 0; j < numSimbolos; j++)
        {
            if (transicionesDet[i][j] == NULL)
            {
                printf("es la i=%d / j=%d\n", i, j);
                return NULL;
            }

            printf("{");
            for (k = 1; k <= transicionesDet[i][j][INDEX]; k++)
            {
                if (transicionesDet[i][j][k] > numEstados)
                {
                    printf("\ntransicionesDet[%d][%d][0] = %d", i, j, transicionesDet[i][j][INDEX]);
                    printf("\nError en el estado de fila %d, columna j=%d. Elemento %d\n", i, j, k);
                    return NULL;
                }
                printf(" %d ", transicionesDet[i][j][k]);
            }
            printf("}\t");
        }
        printf("\n");
    }

    for (i = 0; i < numEstados; i++)
    {
        for (j = 0; j < numSimbolos; j++)
        {
            free(transiciones[i][j]);
        }

        free(transiciones[i]);
    }
    /*TODO liberar inicialDefinitivo y transicionesDet*/

    free(transiciones);

    return afnd;
}

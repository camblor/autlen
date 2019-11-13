#include "../include/transforma.h"

#define INDEX 0

/*
function: estudiarAFND
return: cubo de transiciones del automata finito no determinista.
*/
int ***estudiarAFND(AFND *afnd, int numEstados, int numSimbolos)
{
    int i, j, k;
    int ***transiciones = NULL;

    transiciones = malloc(sizeof(int **) * numEstados);

    printf("    + 0 . l\n");
    printf("   ________\n");
    for (i = 0; i < numEstados; i++)
    {
        printf("q%d| ", i);
        transiciones[i] = malloc(sizeof(int *) * numSimbolos + 1);

        /*Columna: Simbolos*/
        for (j = 0; j <= numSimbolos; j++)
        {
            /*Inicializamos como si no hubiera ninguna transicion*/
            transiciones[i][j] = malloc(sizeof(int));
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
            printf("%d ", transiciones[i][j][INDEX]);
        }
        printf("|\n");
    }
    printf("\n");
    return transiciones;
}
/*
function: obtenerInicial
return: array de enteros con los estados iniciales, lambdas ya tenidas en cuenta
*/
int *obtenerInicial(int ***transiciones, int indiceEstadoInicial, int numSimbolos)
{
    int i;
    int *inicialDefinitivo = NULL;
    inicialDefinitivo = malloc(sizeof(int));
    inicialDefinitivo[INDEX] = 1;
    inicialDefinitivo[inicialDefinitivo[INDEX]] = indiceEstadoInicial;

    /*
    Guardamos estados iniciales aplicadas lambdas:
    inicialDefinitivo[0] = numero de estados iniciales.
    inicialDefinitivo[1] = estado inicial 1
    inicialDefinitivo[2] = estado inicial 2
    ...
    inicialDefinitivo[n] = estado inicial n
    */
    if (transiciones[indiceEstadoInicial][numSimbolos][INDEX] != 0)
    {
        inicialDefinitivo = realloc(inicialDefinitivo, sizeof(inicialDefinitivo) + sizeof(int) * transiciones[indiceEstadoInicial][numSimbolos][INDEX]);
        /*Transiciones lambda*/
        for (i = 0; i < transiciones[indiceEstadoInicial][numSimbolos][INDEX]; i++)
        {
            inicialDefinitivo[INDEX]++;
            inicialDefinitivo[inicialDefinitivo[INDEX]] = transiciones[indiceEstadoInicial][numSimbolos][i];
        }
    }
    return inicialDefinitivo;
}

/*
function: inicialDeterminista
retorno: cubo del AFD con el estado inicial incluido en:

transicionesD[n_estado][n_simbolo]
*/
int ***inicialDeterminista(int *vector, int numSimbolos, int ***transiciones, int fila)
{
    int i, j, k;
    int ***transicionesD;
    transicionesD = malloc(sizeof(int **));
    transicionesD[INDEX] = malloc(sizeof(int *) * (numSimbolos + 1));

    for (i = 1; i <= vector[INDEX]; i++)
    {
        /* Para cada estado inicial del Determinista */
        for (j = 0; j < numSimbolos; j++)
        {
            /*Consultamos todos los simbolos*/
            /*Inicializamos poniendo a 0 el numero de cada transicion*/
            if (transicionesD[fila][j] == NULL)
            {
                transicionesD[fila][j] = malloc(sizeof(int));
                transicionesD[fila][j][fila] = 0;
            }

            /* Si hay alguna transicion desde estado inicial actual con simbolo j*/
            if (transiciones[vector[i]][j][INDEX] != 0)
            {
                /*Reservamos la memoria necesaria para las transiciones que haya*/

                transicionesD[fila][j] = realloc(transicionesD[fila][j], sizeof(transicionesD[fila][j]) + sizeof(int) * transiciones[vector[i]][numSimbolos][INDEX]);

                /*Y las guardamos en la tabla del determinista*/
                for (k = 1; k <= transiciones[vector[i]][j][INDEX]; k++)
                {
                    transicionesD[fila][j][INDEX]++;
                    transicionesD[fila][j][transicionesD[fila][j][INDEX]] = transiciones[vector[i]][j][k];
                }
            }
        }
    }
    /*
    Procesamos estados accesibles desde los estados iniciales

    Tenemos almacenados los estados iniciales en inicialDefinitivo
    Por cada estado en inicialDefinitivo, consultamos sus transiciones con cada simbolo
    y las guardamos en transicionesD[0][j].

    transicionesD[0][j][0] = numero de transiciones de esa celda
    transicionesD[0][j][1] = transicion 1
    transicionesD[0][j][2] = transicion 2
    ...
    transicionesD[0][j][n] = transicion n
   */
    return transicionesD;
}
/*
funcion: crearVisitados
retorno: tabla de visitados
*/
int **crearVisitados(int* inicialDefinitivo)
{
    /* 
    Ya tenemos guardadas en la tabla las transiciones del determinista 
    Ahora recorremos esa tabla hasta encontrar un estado que no haya sido procesado.
    Contamos por estados también los conjuntos de estado (Definicion recursiva)
    */
    int i, j;
    int** visitados;
    visitados = malloc(sizeof(int *));
    visitados[0] = malloc(sizeof(int));
    visitados[0][0] = 1;
    visitados[1] = inicialDefinitivo;

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
    /*
    visitados[0][0] = NUMERO DE ESTADOS VISITADOS
    visitados[i] = ESTADO VISITADO QUE PUEDE ESTAR FORMADO POR VARIOS
    visitados[i][0] = NUMERO DE ELEMENTOS DEL ESTADO
    visitados[i][j] = ELEMENTOS DEL ESTADO   
    
    transicionesD[fila][estado][0] = n transiciones
    transicionesD[fila][estado][x] = transicion
    */
    return visitados;
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
    /* Tabla dinámica para el AFND*/
    int ***transiciones = NULL;
    /* Tabla dinámica para el AFD*/
    int ***transicionesD = NULL;
    int *inicialDefinitivo = NULL;
    /* Tabla que almacena los estados visitados */
    int **visitados = NULL;
    /* Bandera que indica si el estado ya ha sido visitado */
    int flag = 1;

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
    transicionesD = inicialDeterminista(inicialDefinitivo, numSimbolos, transiciones, 0);
    /* Procesado el estado inicial del AFD. */

    
    /* Creamos tabla de conjuntos de estados visitados */
    visitados = crearVisitados(inicialDefinitivo);
    /* Ahora que ya esta creado anadimos a ella los estados que vamos a ir visitando en el bucle principal*/

    

    /* Para cada estado implicado */
    for (i = 0; i < visitados[0][INDEX]; i++)
    {
        printf("FILA %d:\n", i + 1);
        /* Consultar transiciones */
        for (j = 0; j < numSimbolos; j++)
        {
            printf("SIMBOLO: %s - TRANSICIONES: ", AFNDSimboloEn(afnd, j));
            /*Numero de transiciones iniciales == numero de elementos de algun array de visitados*/
            for (k = 1; k <= visitados[0][INDEX]; k++)
            {
                flag = 0;
                /* Si tienen el mismo numero de elementos*/
                if (transicionesD[i][j][INDEX] == visitados[k][INDEX])
                {
                    /*Analizamos elemento a elemento*/
                    for (m = 1; m <= visitados[k][INDEX]; m++)
                    {
                        /*Si algun elemento distinto --> NO ES EL MISMO -> SIGUIENTE (break)*/
                        if (transicionesD[i][j][m] != visitados[k][m])
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

                /*Si ya tenemos este procesado...*/
                if (flag == 1)
                {
                    break;
                }
                /* Si no hay ninguno que haya coincidido como numero de elementos o si haya pero no los elementos internos --> RECORREMOS*/
                /* ACTUALIZAR visitados[0][0] al salir, de forma que este se convierta en el bucle principal*/
                if (k == visitados[0][INDEX] && flag == 0)
                {
                    printf("TOCARIA RECORRER -> %d\n", transicionesD[i][j][INDEX]);
                    /* Para cada simbolo l */
                    for (l = 0; l < numSimbolos; l++)
                    {
                        /* Para cada estado dentro de los conjuntos de estados */
                        for (m = 1; m <= transicionesD[i][l][INDEX]; m++)
                        {
                            printf("%d - %s\n", transicionesD[i][l][m], AFNDSimboloEn(afnd, l));
                        }
                    }

                    /*Acaba de anadirlo*/
                }
            }
        }
    }
    /*
    for(i=0;i<2;i++){
        if(transicionesD[i] == NULL)
                    printf("es la i\n");
        for(j=0;j<numSimbolos;j++){
            if(transicionesD[i][j] == NULL)
                    printf("es la j\n");
            for(k=1;k<=transicionesD[i][j][0];k++){
                
                if(transicionesD[i][j][k] == 0){
                    printf("es la k\n");
                }
                printf("%d ", transicionesD[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }   
*/

    for (i = 0; i < numEstados; i++)
    {
        for (j = 0; j < numSimbolos; j++)
        {
            free(transiciones[i][j]);
        }

        free(transiciones[i]);
    }
    /*TODO liberar inicialDefinitivo y transicionesD*/

    free(transiciones);

    return afnd;
}
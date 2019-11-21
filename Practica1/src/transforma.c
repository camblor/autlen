/*
Archivo: transforma.c
Funcionalidad: Algoritmo y funciones de transformación de un AFND en AFD.
Autores: Alfonso Camblor y Julio Carreras
*/

#include "../include/transforma.h"

/* Tabla dinamica del AFND */
int ***transiciones = NULL;
/* Tabla dinámica para el AFD */
int ***transicionesDet = NULL;
/* Tabla que almacena los estados visitados */
int **visitados = NULL;
int iniciado = 0;
int n_visitados = 0;



/*
Funcion: nuevaFilaDeterminista
Funcionalidad: Introduce una nueva fila al automata finito determinista
Argumentos:
    int* vector = vector que quieres introducir en el automata como nueva fila;
    int numSimbolos = numSimbolos;
    int ***transiciones = tabla de transiciones del automata finito no determinista;
    int fila = indice de la nueva fila en el automata finito determinista.
*/
int ***nuevaFilaDeterminista(int *vector, int numSimbolos, int ***transiciones, int fila)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    int n = 0;
    int flag = 0;
    int flag2 = 0;

    if (vector == NULL || transiciones == NULL)
    {
        fprintf(stderr, "Error de argumentos: nuevaFilaDeterminista");
        return NULL;
    }

    /* Memoria */

    transicionesDet = (int ***)realloc(transicionesDet, (fila + 1) * sizeof(int **));
    if (transicionesDet == NULL)
    {
        return NULL;
    }
    transicionesDet[fila] = (int **)malloc(sizeof(int *) * (numSimbolos + 1));
    if (transicionesDet[fila] == NULL)
    {
        return NULL;
    }

    for (n = 0; n < numSimbolos; n++)
    {

        transicionesDet[fila][n] = malloc(sizeof(int));

        if (transicionesDet[fila][n] == NULL)
        {
            return NULL;
        }
        transicionesDet[fila][n][INDEX] = 0;
    }

    /* Ver entrada */
    imprimeVectorFila(vector, fila);

    for (i = 1; i <= vector[INDEX]; i++)
    {
        /*printf("Estamos evaluando %d\n", vector[i]);*/
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
                        printf("YA ESTA\n");
                        flag = 1;
                    }
                }
                /*flag = compararVectores(transiciones[vector[i]][j],transicionesDet[fila][j] );*/

                if (flag != 1 && transiciones[vector[i]][j][INDEX] != -1)
                {

                    /*printf("ESTAMOS AÑADIENDO a la fila %d simbolo %d la transicion a %d\n",fila,j, transiciones[vector[i]][j][k]);*/

                    transicionesDet[fila][j][INDEX]++;
                    transicionesDet[fila][j] = realloc(transicionesDet[fila][j], (transicionesDet[fila][j][INDEX] * sizeof(int)) + sizeof(int));
                    transicionesDet[fila][j][transicionesDet[fila][j][INDEX]] = transiciones[vector[i]][j][k];

                    /* Comprobar lambdas para el estado añadido transiciones[vector[i]][j][k]. */
                    for (m = 1; m <= transiciones[transiciones[vector[i]][j][k]][numSimbolos][INDEX]; m++)
                    {

                        for (n = 1; n <= transicionesDet[fila][j][INDEX]; n++)
                        {
                            flag2 = 0;
                            if (transicionesDet[fila][j][n] == transiciones[transiciones[vector[i]][j][k]][numSimbolos][m])
                            {
                                flag2 = 1;
                            }
                            if (!flag2 && !contiene(transicionesDet[fila][j], transiciones[transiciones[vector[i]][j][k]][numSimbolos][m]))
                            {
                                transicionesDet[fila][j][INDEX]++;
                                transicionesDet[fila][j] = realloc(transicionesDet[fila][j], (transicionesDet[fila][j][INDEX] * sizeof(int)) + sizeof(int));
                                transicionesDet[fila][j][transicionesDet[fila][j][INDEX]] = transiciones[transiciones[vector[i]][j][k]][numSimbolos][m];
                            }
                        }
                        /*
                        printf("DESPUES:\n");
                        imprimeVectorFila(transicionesDet[fila][j], fila);
                        */
                    }
                }
            }
        }
    }
    return transicionesDet;
}

/* 
Funcion: AFNDTransforma
Funcionalidad: 
    Transforma un automata finito no determinista pasado como argumento
    y lo transforma en un automata finito determinista de formato AFND.
Argumentos:
    AFND* anfd = Automata finito no determinista a transformar en AFD:
*/
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
    /* Datos generales del AFND */
    int numSimbolos = AFNDNumSimbolos(afnd);
    int numEstados = AFNDNumEstados(afnd);

    int *inicialDefinitivo = NULL;

    /* Bandera que indica si el estado ya ha sido visitado */
    int flag = 0;
    int count = 0;
    int fila = 0;
    int *temporal = NULL;

    /* Puntero del AFD */
    AFND *afd = NULL;


    /*Meter nombres en creaAFND*/
    char *nombre;
    char *temp2;
    int tipoEstado = NORMAL;
    int estadoDefinitivo = NORMAL;
    int yaInicial = 0;



    /*
    ----------------------------------
            ESTUDIO DEL AFND
    ----------------------------------
    

    Metemos la tabla de transiciones del AFND en transiciones.
    */
    transiciones = estudiarAFND(afnd, numEstados, numSimbolos);

    /* Ahora debemos rellenar la tabla del AFD. */

    /*
    ----------------------------------
            E.Inicial del AFD
    ----------------------------------
    
    Obtenemos el estado inicial teniendo en cuenta lambdas. 
    */
    inicialDefinitivo = obtenerInicial(transiciones, AFNDIndiceEstadoInicial(afnd), numSimbolos, &iniciado);

    /*
    ----------------------------------
            CREACION TABLA DEL AFD
    ----------------------------------
    

    Metemos la tabla de transiciones del AFND en transiciones y procesamos su El estado inicial del AFD.
    */
    transicionesDet = inicialDeterminista(inicialDefinitivo, numSimbolos, transiciones, 0);
    fila++;
    n_visitados++;

    /* Creamos tabla de conjuntos de estados visitados */
    visitados = crearVisitados(inicialDefinitivo);

    /* Ahora que ya esta creado anadimos a ella los estados que vamos a ir visitando en el bucle principal*/
    imprimirAFND(transiciones, numEstados, numSimbolos);

    /*
    ----------------------------------
            BUCLE PRINCIPAL
    ----------------------------------
    
    
    Vamos visitando y rellenando la tabla del AFD.
    */
    for (i = 0; i < visitados[INDEX][INDEX]; i++)
    {

        /* Para cada estado i */
        for (j = 0; j < numSimbolos; j++)
        {
            /* Introducimos el simbolo j */

            /*
                FASE DE COMPROBACION DE SI EL VECTOR TRANSICIONESDET[I][J] ya ha sido visitado 
            */
            flag = 0;
            if (transicionesDet[i][j][INDEX] <= 0)
            {
                flag = 1;
                count = 0;
            }
            for (k = 1; k <= visitados[INDEX][INDEX] && flag != 1; k++)
            {
                /*
                COMPARANDO
                imprimeVectorFila(visitados[k], 100);
                imprimeVectorFila(transicionesDet[i][j], 100);
                */
                count = compararVectores(visitados[k], transicionesDet[i][j]);

                /* Si visitado --> pasamos al siguiente */
                if (count == 0)
                {
                    break;
                }
            }
            /* Si no visitado --> visitamos y lo metemos a la tabla de visitados*/
            if (count != 0)
            {
                /* Para cada estado dentro de los conjuntos de estados */
                temporal = NULL;

                for (l = 1; l <= transicionesDet[i][j][INDEX]; l++)
                {
                    if (l == 1)
                    {
                        temporal = obtenerInicial(transiciones, transicionesDet[i][j][l], numSimbolos, &iniciado);
                        /*
                        printf("INICIADO VECTOR: ");
                        imprimeVectorFila(temporal, 900);
                        */
                    }
                    if (temporal != NULL)
                    {
                        
                        temporal = anadirTransiciones(temporal, transicionesDet[i][j][l], numSimbolos, transiciones);
                        /*
                        printf("NUEVATRANSICION: ");
                        imprimeVectorFila(temporal, 900);
                        */
                    }
                }
                for (k = 1; k <= visitados[INDEX][INDEX] && flag != 1; k++)
                {

                    count = compararVectores(visitados[k], temporal);

                    if (count == 0)
                    {
                        free(temporal);
                        temporal = NULL;
                        break;
                    }
                }

                if (temporal != NULL)
                {
                    /*
                    printf("AÑADIDO A LA TABLADET(%d,%d) ", fila, numSimbolos);
                    imprimeVectorFila(temporal, 500);
                    */
                    transicionesDet = nuevaFilaDeterminista(temporal, numSimbolos, transiciones, fila++);
                    printf("FILA=%d\n", fila);
                    if (!transicionesDet)
                    {
                        return NULL;
                    }
                    mergeSort(temporal, 1, temporal[0]);
                    visitados = addVisitado(temporal, &n_visitados, visitados);
                    imprimeVisitados(visitados, n_visitados);
                }
            }
        }
    }
    
        for (i = 0; i < fila; i++)
    {
        if (transicionesDet[i] == NULL)
        {
            printf("AYUDA EN %d", i);
            return NULL;
        }
        printf("fila %d\n", i);

        for (j = 0; j < numSimbolos; j++)
        {
            if (transicionesDet[i][j] == NULL)
            {
                printf("AYUDA EN %d-%d\n", i, j);
                return NULL;
            }
            mergeSort(transicionesDet[i][j], 1, transicionesDet[i][j][INDEX]);
            printf("Simbolo %s: {", AFNDSimboloEn(afnd, j));
            for (k = 1; k <= transicionesDet[i][j][INDEX]; k++)
            {
                printf(" %d ", transicionesDet[i][j][k]);
            }
            printf("}\n");
        }
        printf("\n");
    }

    /*
    ----------------------------------
            EJECUTAR EL TRANSFORMA
    ----------------------------------
    
    Generamos la transformacion del AFND en AFD.
    */

    afd = AFNDNuevo("afd", fila, numSimbolos);

    for (i = 0; i < numSimbolos; i++)
    {
        AFNDInsertaSimbolo(afd, AFNDSimboloEn(afnd, i));
    }

    /*Aqui tenemos los nombres de los estados*/

    for (i = 0; i < fila; i++)
    {
        estadoDefinitivo = NORMAL;
        nombre = malloc(sizeof(char) * 2);

        strcpy(nombre, "q");

        for (l = 1, tipoEstado = NORMAL; l <= visitados[i + 1][INDEX]; l++)
        {
            tipoEstado = AFNDTipoEstadoEn(afnd, visitados[i + 1][l]);

            /* INICIALYFINAL */
            if (((tipoEstado == INICIAL_Y_FINAL && estadoDefinitivo != INICIAL_Y_FINAL) || (tipoEstado == INICIAL && estadoDefinitivo == FINAL) || (tipoEstado == FINAL && estadoDefinitivo == INICIAL)) && !yaInicial)
            {
                estadoDefinitivo = INICIAL_Y_FINAL;
                yaInicial = 1;
            }
            /* INICIAL */
            else if (tipoEstado == INICIAL && estadoDefinitivo == NORMAL && !yaInicial)
            {
                estadoDefinitivo = INICIAL;
                yaInicial = 1;
            }
            /* FINAL */
            else if (tipoEstado == FINAL && estadoDefinitivo == NORMAL)
            {
                estadoDefinitivo = FINAL;
            }

            temp2 = malloc(sizeof(char) * 2);
            nombre = realloc(nombre, sizeof(nombre) + sizeof(char));
            sprintf(temp2, "%d", visitados[i + 1][l]);
            strcat(nombre, temp2);
            free(temp2);
        }

        AFNDInsertaEstado(afd, nombre, estadoDefinitivo);
        free(nombre);
    }

    /*Aqui tenemos las transiciones*/
    for (i = 0; i < fila; i++)
    {

        for (j = 0; j < numSimbolos; j++)
        {

            /*la i y la j funcionan correctamente.*/

            /*Para cada celda recorrer vector y crear nombre estado.*/
            nombre = malloc(sizeof(char) * 2);
            strcpy(nombre, "q");
            flag = 0;
            for (k = 1; k <= transicionesDet[i][j][INDEX]; k++)
            {
                flag = 1;

                temp2 = malloc(sizeof(char) * 2);
                nombre = realloc(nombre, sizeof(nombre) + sizeof(temp2));
                sprintf(temp2, "%d", transicionesDet[i][j][k]);
                strcat(nombre, temp2);
                free(temp2);
            }

            if (flag == 1 && transicionesDet[i][j][INDEX] > 0 && AFNDIndiceDeEstado(afd, nombre) != -1)
            {
                /*printf("TRANSICION AÑADIDA: %s -> %s mediante %s\n", AFNDNombreEstadoEn(afd, i), nombre, AFNDSimboloEn(afd, j));*/

                AFNDInsertaTransicion(afd, AFNDNombreEstadoEn(afd, i), AFNDSimboloEn(afd, j), nombre);
            }

            /* Aqui ya esta creado*/
            free(nombre);
        }
    }



    /*Dado que no hay transiciones lambda por ser un AFD, lo tenemos listo para devolver.*/

    /*
    ----------------------------------
            LIBERAR MEMORIA
    ----------------------------------
    
    Liberamos la memoria utilizada.
    */

    /* Liberamos la tabla de transiciones del AFND. */
    for (i = 0; i < numEstados; i++)
    {
        for (j = 0; j <= numSimbolos; j++)
        {
            free(transiciones[i][j]);
        }

        free(transiciones[i]);
    }
    free(transiciones);

    /* Liberamos la tabla de transiciones del AFD. */
    for (i = 0; i < fila; i++)
    {
        for (j = 0; j < numSimbolos; j++)
        {
            free(transicionesDet[i][j]);
        }
        free(transicionesDet[i]);
    }
    free(transicionesDet);

    /* Liberamos la tabla de visitados */
    for (i = 1; i <= visitados[INDEX][INDEX]; i++)
    {
        free(visitados[i]);
    }
    free(visitados[INDEX]);
    free(visitados);

    return afd;
}

#include "../include/transforma.h"

AFND *AFNDTransforma(AFND *afnd)
{
    int numSimbolos = AFNDNumSimbolos(afnd);
    int numEstados = AFNDNumEstados(afnd);
    int indiceEstadoInicial = AFNDIndiceEstadoInicial(afnd);
    int estado;
    int simbolo;
    int i, j, k;

    /* Estado inicial */

    int *inicialDeterministaIndex = NULL;
    int numIniciales = 0;
    char *inicialDeterminista = NULL;

    /* Tabla dinámica para el AFND*/
    int ***transiciones = NULL;
    /* Tabla dinámica para el AFD*/
    int ***transicionesD = NULL;
    int *inicialDefinitivo = NULL;

    /*Reserva de memoria*/
    transiciones = malloc(sizeof(int **) * numEstados);
    transicionesD = malloc(sizeof(int **));
    transicionesD[0] = malloc(sizeof(int *) * (numSimbolos + 1));
    /*hasta aqui tienes para puntero en cada estado/simbolo*/

    /*
    ----------------------------------
            ESTUDIO DEL AFND
    ----------------------------------    
    
    Tabla de transiciones con los estados por filas
    y en [0] de cada celda el numero de transiciones,
    seguido de las transiciones hasta la indicada.
    */
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
            transiciones[i][j][0] = 0;
            for (estado = 0; estado < numEstados; estado++)
            {

                /*Se guardan las transiciones por simbolos de cada estado en la tabla*/
                if (j < numSimbolos && AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, j, estado))
                {
                    transiciones[i][j] = realloc(transiciones[i][j], sizeof(transiciones[i][j]) + sizeof(int));
                    transiciones[i][j][0]++;
                    transiciones[i][j][transiciones[i][j][0]] = estado;
                }
                /*Se guardan las transiciones lambda de cada estado en la tabla*/
                else if (j == numSimbolos && i != estado && AFNDCierreLTransicionIJ(afnd, i, estado))
                {

                    transiciones[i][j] = realloc(transiciones[i][j], sizeof(transiciones[i][j]) + sizeof(int));
                    transiciones[i][j][0]++;
                    transiciones[i][j][transiciones[i][j][0]] = estado;
                }
            }
            printf("%d ", transiciones[i][j][0]);
        }
        printf("|\n");
    }

    /*Al llegar aqui tenemos la tabla de estados del AFND y nos queda rellenar la del AFD*/

    /*Primero debemos obtener el estado inicial del AFND*/
    /*Columna lambda = numSimbolos*/

    /*Guardamos el inicial y pasamos a procesar las lambdas*/
    inicialDefinitivo = malloc(sizeof(int));
    inicialDefinitivo[0] = 1;
    inicialDefinitivo[inicialDefinitivo[0]] = indiceEstadoInicial;

    /*
    Guardamos estados iniciales aplicadas lambdas:
    inicialDefinitivo[0] = numero de estados iniciales.
    inicialDefinitivo[1] = estado inicial 1
    inicialDefinitivo[2] = estado inicial 2
    ...
    inicialDefinitivo[n] = estado inicial n
    */
    if (transiciones[indiceEstadoInicial][numSimbolos][0] != 0)
    {
        inicialDefinitivo = realloc(inicialDefinitivo, sizeof(inicialDefinitivo) + sizeof(int) * transiciones[indiceEstadoInicial][numSimbolos][0]);
        /*Transiciones lambda*/
        for (i = 0; i < transiciones[indiceEstadoInicial][numSimbolos][0]; i++)
        {
            inicialDefinitivo[0]++;
            inicialDefinitivo[inicialDefinitivo[0]] = transiciones[indiceEstadoInicial][numSimbolos][i];
        }
    }

    /*Procesamos estados accesibles desde los estados iniciales*/
    /*
    Tenemos almacenados los estados iniciales en inicialDefinitivo
    Por cada estado en inicialDefinitivo, consultamos sus transiciones con cada simbolo
    y las guardamos en transicionesD[0][j].
    transicionesD[0][j][0] = numero de transiciones de esa celda
    transicionesD[0][j][1] = transicion 1
    transicionesD[0][j][2] = transicion 2
    ...
    transicionesD[0][j][n] = transicion n
   */
    for (i = 1; i <= inicialDefinitivo[0]; i++)
    {
        /* Para cada estado inicial del Determinista */
        for (j = 0; j < numSimbolos; j++)
        {
            /*Consultamos todos los simbolos*/
            /*Inicializamos poniendo a 0 el numero de cada transicion*/
            if (transicionesD[0][j] == NULL)
            {
                transicionesD[0][j] = malloc(sizeof(int));
                transicionesD[0][j][0] = 0;
            }
            /* Si hay alguna transicion desde estado inicial actual con simbolo Sj*/
            if (transiciones[inicialDefinitivo[i]][j][0] != 0)
            {
                /*Reservamos la memoria necesaria para las transiciones que haya*/

                transicionesD[0][j] = realloc(transicionesD[0][j], sizeof(transicionesD[0][j]) + sizeof(int) * transiciones[inicialDefinitivo[i]][numSimbolos][0]);

                /*Y las guardamos en la tabla del determinista*/
                for (k = 1; k <= transiciones[inicialDefinitivo[i]][j][0]; k++)
                {
                    transicionesD[0][j][0]++;
                    transicionesD[0][j][transicionesD[0][j][0]] = transiciones[inicialDefinitivo[i]][j][k];
                }
            }
        }
    }
    /*Fila completada*/

    /* 
    Ya tenemos guardadas en la tabla las transiciones del determinista 
    Ahora recorremos esa tabla hasta encontrar un estado que no haya sido procesado.
    Contamos por estados también los conjuntos de estado (Definicion recursiva)
    */

    /*
   for estado in fila_de_transicionesD[Primera a ultima]:{

       Si estado no procesado{
           hallar los estados a los que transita.
       }
   }    
   */

    int **visitados = NULL;
    int l, m;
    int flag = 1;
    visitados = malloc(sizeof(int *));
    visitados[0] = malloc(sizeof(int));
    visitados[0][0] = 1;
    visitados[1] = inicialDefinitivo;
    int numerofilas = 1;
    /*
    for (int l = 1; l <= visitados[1][0]; l++)
    {
        printf("inicial: %d", inicialDefinitivo[l]);
    }*/



    /* 
    transicionesD[fila][estado][0] = n transiciones
    transicionesD[fila][estado][x] = transicion
    */

    /* Para cada estado implicado */
    for (i = 0; i < visitados[0][0]; i++)
    {
        printf("FILA %d:\n", i + 1);
        /* Consultar transiciones */
        for (j = 0; j < numSimbolos; j++)
        {
            printf("SIMBOLO: %s - TRANSICIONES: ", AFNDSimboloEn(afnd, j));
            /*Numero de transiciones iniciales == numero de elementos de algun array de visitados*/
            for (l = 1; l <= visitados[0][0]; l++)
            {
                flag = 0;
                /* Si tienen el mismo numero de elementos*/
                if (transicionesD[i][j][0] == visitados[l][0])
                {
                    /*Analizamos elemento a elemento*/
                    for (m = 1; m <= visitados[l][0]; m++)
                    {
                        /*Si algun elemento distinto --> NO ES EL MISMO -> SIGUIENTE (break)*/
                        if (transicionesD[i][j][m] != visitados[l][m])
                        {
                            flag = 0;
                            break;
                        }
                        /* Si todos los elementos son iguales --> VISITADO flag = 1*/
                        else if (m == visitados[l][0])
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
                if (l == visitados[0][0] && flag == 0)
                {
                    printf("TOCARIA RECORRER -> %d\n", transicionesD[i][j][0]);
                    /* Para cada estado de la celda recorrer cada simbolo para cada elemento del conjunto.*/
                    for(int n = 0; n< numSimbolos; n++){
                        for(int o = 1; o<=transicionesD[i][j][0];o++){
                            printf("%d-", transicionesD[i][j][o]);
                            /*
                            Comprueba bien para cada simbolo, los elementos el conjunto.
                            Falta introducir los estados a los que llega e incrementar el contador del bucle tocho.
                            */
                        }
                        printf("\n");
                    }
                    
                    /*Acaba de anadirlo*/
                }
            }
        }
    }

    for (estado = 0; estado < numEstados; estado++)
    {
        for (int simbolo = 0; simbolo < numSimbolos; simbolo++)
        {
            free(transiciones[estado][simbolo]);
        }
        
        free(transiciones[estado]);
    }
    /*TODO liberar inicialDefinitivo y transicionesD*/

    free(transiciones);
    free(inicialDeterminista);

    return afnd;
}

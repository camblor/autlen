/*
Archivo: auxiliar.h
Funcionalidad: Funciones auxiliares para transforma.c
Autores: Alfonso Camblor y Julio Carreras
*/
#ifndef AFND_AUX
#define AFND_AUX
#include "transforma.h"
#define INDEX 0

/*
Funcion: mergeSort
Funcionalidad: Algoritmo MergeSort.
Argumentos:
    int* arr: array a ordenar
    int l: primer indice.
    int r: ultimo indice.
*/
void mergeSort(int * arr, int l, int r);

/*
Funcion: compararVectores
Funcionalidad: compara dos vectores interpretandolos como conjuntos.
Argumentos:
    int* vector1: vector a comparar 1
    int* vector2: vector a comparar 2
*/
int compararVectores(int* vector1, int*vector2);


/*
Funcion: imprimirAFND
Funcionalidad: Imprime la tabla del automata finito no determinista.
Argumentos:
    int*** transiciones = tabla de transiciones del AFND.
    int numEstados = numEstados del AFND.
    int numSimbolos = numSimbolos del AFND.
*/
void imprimirAFND(int ***transiciones, int numEstados, int numSimbolos);

/*
Funcion: estudiarAFND
Funcionalidad: cubo de transiciones del automata finito no determinista.
Argumentos:
    AFND* afnd = automata finito no determinista creado y listo para ser transformado.
    int numEstados = numEstados del AFND.
    int numSimbolos = numSimbolos.
*/
int ***estudiarAFND(AFND *afnd, int numEstados, int numSimbolos);

/*
Funcion: contiene
Funcionalidad: Comprueba si el vector tiene el estado almacenado.
Argumentos:
    int* vector = vector donde se comprobara.
    int estado = estado a comprobar su aparicion en el vector.
*/
int contiene(int *vector, int estado);

/*
Funcion: imprimeVectorFila
Funcionalidad: Imprime por pantalla el vector con un identificador llamado fila por su aplicacion en el debugging
Argumentos:
    int* vector = vector a imprimir.
    int fila = identificador de impresion.
*/
void imprimeVectorFila(int *vector, int fila);

/*
Funcion: anadirTransiciones
Funcionalidad: Introduce en el vector un nuevo estado del AFD
Argumentos:
    int* vector = Vector a introducir en la tabla de transiciones.
    int indiceEstado = Indice del estado a introducir en el vector.
    int numSimbolos = numSimbolos.
    int*** transiciones = tabla de transiciones del AFND.
*/
int *anadirTransiciones(int *vector, int indiceEstado, int numSimbolos, int*** transicionesT);

/*
Funcion: addVisitado
Funcionalidad: Introduce un nuevo vector en la tabla de visitados
Argumentos:
    int* vector = vector a introducir.
    int* n_visitados = direccion de memoria de n_visitados.
    int **visitados = lista de estados visitados.
*/
int **addVisitado(int *vector, int *n_visitados, int**visitados);

/*
Funcion: imprimeVisitados
Funcionalidad: Imprime la tabla de los estados visitados.
Argumentos:
    tabla de estados visitados.
*/
void imprimeVisitados(int **visitados, int n_visitados);


/*
Funcion: crearVisitados
Funcionalidad: Creacion de la tabla de visitados.
Argumentos:
    int* vector = vector a introducir como primero visitado.
*/
int **crearVisitados(int *vector);


/*
Funcion: inicialDeterminista
Funcionalidad: Crea el cubo del AFD con el estado inicial ya visitado y procesadas las transiciones lambda.
Argumentos:
    int* vector = vector de la primera fila de la tabla de transiciones del AFD.
    int numSimbolos = numSimbolos.
    int*** transiciones = tabla de transiciones del AFND.
    int fila = indice de la fila en la tabla del AFD.
*/
int ***inicialDeterminista(int *vector, int numSimbolos, int ***transiciones, int fila);


/*
Funcion: obtenerInicial
Funcionalidad: Obtiene el vector inicial a partir del que comienza el algoritmo.
Argumentos:
    int*** transiciones = tabla de transiciones del AFND.
    int indiceEstado = estado inicial del AFND.
    int numSimbolos = numero de simbolos del AFND.
    int* iniciado = variable auxiliar.
return: array de enteros con los estados iniciales, lambdas ya tenidas en cuenta
*/
int *obtenerInicial(int ***transiciones, int indiceEstado, int numSimbolos, int *iniciado);

/*
Funcion: imprimirAFD
Funcionalidad: imprime el AFND tal y como se recibe en la estructura intermedia 2 (tabla de transiciones del AFD)
Argumentos:
    int transicionesDet = tabla de transiciones del AFD.
    AFND *afnd = AFND Original.
    int fila = numero de filas del AFD.
    int numSimbolos = numero de simbolos del AFND Original.
*/
void imprimeAFD(int ***transicionesDet, AFND *afnd, int fila, int numSimbolos);


#endif
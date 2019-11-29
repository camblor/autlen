/*
Archivo: transforma.h
Funcionalidad: Algoritmo y funciones de transformación de un AFND en AFD.
Autores: Alfonso Camblor y Julio Carreras
*/

#ifndef AFND_T
#define AFND_T
#include "afnd.h"

/*
Funcion: AFNDMinimiza
Funcionalidad:
    Recibe un automata finito pasado como argumento
    y lo reduce al mínimo número de estados necesarios.
Argumentos:
    AFND* anfd = Automata finito a minimizar (formato afnd):
*/
int *finalesAFND(AFND *afnd, int numEstados);

AFND * AFNDMinimiza(AFND * afnd);

#endif

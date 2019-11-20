#ifndef AFND_T
#define AFND_T
#include "afnd.h"
#include "auxiliar.h"

/* 
Funcion: AFNDTransforma
Funcionalidad: 
    Transforma un automata finito no determinista pasado como argumento
    y lo transforma en un automata finito determinista de formato AFND.
Argumentos:
    AFND* anfd = Automata finito no determinista a transformar en AFD:
*/
AFND * AFNDTransforma(AFND * afnd);

#endif
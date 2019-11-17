
#include "../include/transforma.h"
/*#include "transforma.h"*/

int main(int argc, char ** argv){

	AFND * p_afnd;
	AFND * afd;
	
	p_afnd = AFNDNuevo("afnd", 5, 4);
	
    /* SIMBOLOS Y ESTADOS */
	AFNDInsertaSimbolo(p_afnd,"a");
	AFNDInsertaSimbolo(p_afnd, "b");
	AFNDInsertaSimbolo(p_afnd,"c");
    AFNDInsertaSimbolo(p_afnd,"d");
	
	AFNDInsertaEstado(p_afnd, "q0", INICIAL_Y_FINAL);
    AFNDInsertaEstado(p_afnd, "q1", NORMAL);
    AFNDInsertaEstado(p_afnd, "q2", FINAL);
    AFNDInsertaEstado(p_afnd, "q3", NORMAL);
    AFNDInsertaEstado(p_afnd, "q4", NORMAL);
    
    /*TRANSICIONES*/
    AFNDInsertaTransicion(p_afnd, "q0", "a", "q1");
    AFNDInsertaTransicion(p_afnd, "q0", "a", "q3");

    AFNDInsertaTransicion(p_afnd, "q1", "b", "q0");
    AFNDInsertaTransicion(p_afnd, "q1", "c", "q1");
    AFNDInsertaTransicion(p_afnd, "q1", "c", "q2");

    AFNDInsertaTransicion(p_afnd, "q2", "d", "q4");

    AFNDInsertaTransicion(p_afnd, "q3", "a", "q3");
    AFNDInsertaTransicion(p_afnd, "q3", "d", "q4");
    

    /*TRANSICIONES LAMBDA*/
    AFNDInsertaLTransicion(p_afnd, "q0", "q1");
    AFNDInsertaLTransicion(p_afnd, "q4", "q0");
    AFNDInsertaLTransicion(p_afnd, "q4", "q2");
    AFNDCierraLTransicion(p_afnd);

    /* MOSTRAR */
    AFNDADot(p_afnd);
	printf("\n");

	afd  = AFNDTransforma(p_afnd);
	AFNDADot(afd);
	printf("\n");
	
	AFNDElimina(afd);
	AFNDElimina(p_afnd);

    return 0;
}
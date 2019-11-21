
#include "../include/transforma.h"
/*#include "transforma.h"*/


int main(int argc, char ** argv)
{

	AFND * p_afnd;
	AFND * afd;
	
	p_afnd= AFNDNuevo("af11", 7, 3);
	
	AFNDInsertaSimbolo(p_afnd,"+");
	AFNDInsertaSimbolo(p_afnd, "0");
	AFNDInsertaSimbolo(p_afnd,".");
	
	AFNDInsertaEstado(p_afnd, "ESTADO",INICIAL_Y_FINAL);
	AFNDInsertaEstado(p_afnd, "q1", NORMAL);
	AFNDInsertaEstado(p_afnd, "q2", NORMAL);
	AFNDInsertaEstado(p_afnd, "q3", NORMAL);
	AFNDInsertaEstado(p_afnd, "q4", NORMAL);
	AFNDInsertaEstado(p_afnd, "q5", FINAL);
	AFNDInsertaEstado(p_afnd, "q6", NORMAL);
	
	AFNDInsertaTransicion(p_afnd, "ESTADO", "+", "q1");
	AFNDInsertaTransicion(p_afnd, "q1", "0", "q1");
	AFNDInsertaTransicion(p_afnd, "q1", "0", "q4");
	AFNDInsertaTransicion(p_afnd, "q1", ".", "q2");
	AFNDInsertaTransicion(p_afnd, "q2", "0", "q3");
	AFNDInsertaTransicion(p_afnd, "q3", "0", "q3");
	AFNDInsertaTransicion(p_afnd, "q4", ".", "q3");
	AFNDInsertaTransicion(p_afnd, "q6", ".", "q3");
	AFNDInsertaTransicion(p_afnd, "q6", "0", "q1");
	
	AFNDInsertaLTransicion(p_afnd, "ESTADO", "q1");
	AFNDInsertaLTransicion(p_afnd, "q1", "q6");
	AFNDInsertaLTransicion(p_afnd, "q3", "q5");
	AFNDInsertaLTransicion(p_afnd, "q5", "q6");
	AFNDCierraLTransicion(p_afnd);

	afd  = AFNDTransforma(p_afnd);
	AFNDImprime(stdout,afd);
	AFNDADot(afd);

	AFNDElimina(afd);
	AFNDElimina(p_afnd);

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/transforma.h"

int main1(int argc, char ** argv)
{

	AFND * p_afnd;
	AFND * afd;
	
	p_afnd= AFNDNuevo("afnd", 7, 3);
	
	AFNDInsertaSimbolo(p_afnd,"+");
	AFNDInsertaSimbolo(p_afnd, "-");
	AFNDInsertaSimbolo(p_afnd,",");
	
	AFNDInsertaEstado(p_afnd, "q0",INICIAL);
	AFNDInsertaEstado(p_afnd, "q1", NORMAL);
	AFNDInsertaEstado(p_afnd, "q2", NORMAL);
	AFNDInsertaEstado(p_afnd, "q3", NORMAL);
	AFNDInsertaEstado(p_afnd, "q4", FINAL);
	AFNDInsertaEstado(p_afnd, "q5", NORMAL);
    AFNDInsertaEstado(p_afnd, "q6", FINAL);
	
	AFNDInsertaTransicion(p_afnd, "q0", "+", "q1");
    AFNDInsertaTransicion(p_afnd, "q0", "+", "q0");
    AFNDInsertaTransicion(p_afnd, "q0", "-", "q2");
	AFNDInsertaTransicion(p_afnd, "q1", "-", "q1");
	AFNDInsertaTransicion(p_afnd, "q1", "-", "q3");
	AFNDInsertaTransicion(p_afnd, "q2", ",", "q4");
	AFNDInsertaTransicion(p_afnd, "q3", "+", "q1");
	AFNDInsertaTransicion(p_afnd, "q4", ",", "q5");
	AFNDInsertaTransicion(p_afnd, "q4", "-", "q5");
    AFNDInsertaTransicion(p_afnd, "q5", "+", "q5");
    AFNDInsertaTransicion(p_afnd, "q5", "+", "q6");
	AFNDInsertaTransicion(p_afnd, "q5", "-", "q5");
	
	AFNDInsertaLTransicion(p_afnd, "q0", "q6");
	AFNDInsertaLTransicion(p_afnd, "q4", "q6");
    AFNDInsertaLTransicion(p_afnd, "q3", "q4");
	AFNDCierraLTransicion(p_afnd);

	afd  = AFNDTransforma(p_afnd);
	AFNDImprime(stdout,afd);
	AFNDADot(afd);
    AFNDADot(p_afnd);
	
	AFNDElimina(afd);
	AFNDElimina(p_afnd);

	return 0;
}
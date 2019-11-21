
#include "../include/transforma.h"


int main(int argc, char ** argv)
{

	AFND * p_afnd;
	AFND * afd;
	
	p_afnd= AFNDNuevo("afnd", 7, 3);
	
	AFNDInsertaSimbolo(p_afnd,"+");
	AFNDInsertaSimbolo(p_afnd, "transicion");
	AFNDInsertaSimbolo(p_afnd,".");
	
	AFNDInsertaEstado(p_afnd, "ESTADO",INICIAL_Y_FINAL);
	AFNDInsertaEstado(p_afnd, "q1", NORMAL);
	AFNDInsertaEstado(p_afnd, "q2", NORMAL);
	AFNDInsertaEstado(p_afnd, "q3", NORMAL);
	AFNDInsertaEstado(p_afnd, "q4", NORMAL);
	AFNDInsertaEstado(p_afnd, "q5", FINAL);
	AFNDInsertaEstado(p_afnd, "q6", NORMAL);
	
	AFNDInsertaTransicion(p_afnd, "ESTADO", "+", "q1");
	AFNDInsertaTransicion(p_afnd, "q1", "transicion", "q1");
	AFNDInsertaTransicion(p_afnd, "q1", "transicion", "q4");
	AFNDInsertaTransicion(p_afnd, "q1", ".", "q2");
	AFNDInsertaTransicion(p_afnd, "q2", "transicion", "q3");
	AFNDInsertaTransicion(p_afnd, "q3", "transicion", "q3");
	AFNDInsertaTransicion(p_afnd, "q4", ".", "q3");
	AFNDInsertaTransicion(p_afnd, "q6", ".", "q3");
	AFNDInsertaTransicion(p_afnd, "q6", "transicion", "q1");
	
	AFNDInsertaLTransicion(p_afnd, "ESTADO", "q1");
	AFNDInsertaLTransicion(p_afnd, "q1", "q6");
	AFNDInsertaLTransicion(p_afnd, "q3", "q5");
	AFNDInsertaLTransicion(p_afnd, "q5", "q6");
	AFNDCierraLTransicion(p_afnd);

	afd  = AFNDTransforma(p_afnd);
	AFNDADot(afd);
	AFNDADot(p_afnd);

	AFNDElimina(afd);
	AFNDElimina(p_afnd);

	return 0;
}
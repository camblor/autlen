#include <stdio.h>

#include "transforma.h"

AFND * AFNDTransforma(AFND * p_afnd){
  AFND * afd;

  afd= AFNDNuevo("afd", 6, 3);

  AFNDInsertaSimbolo(afd,"+");
  AFNDInsertaSimbolo(afd, "0");
  AFNDInsertaSimbolo(afd,".");

  AFNDInsertaEstado(afd, "q0",INICIAL);
  AFNDInsertaEstado(afd, "q1", NORMAL);
  AFNDInsertaEstado(afd, "q2", NORMAL);
  AFNDInsertaEstado(afd, "q3", NORMAL);
  AFNDInsertaEstado(afd, "q4", NORMAL);
  AFNDInsertaEstado(afd, "q5", FINAL);

  AFNDInsertaTransicion(afd, "q0", "+", "q1");
  AFNDInsertaTransicion(afd, "q1", "0", "q1");
  AFNDInsertaTransicion(afd, "q1", "0", "q4");
  AFNDInsertaTransicion(afd, "q1", ".", "q2");
  AFNDInsertaTransicion(afd, "q2", "0", "q3");
  AFNDInsertaTransicion(afd, "q3", "0", "q3");
  AFNDInsertaTransicion(afd, "q4", ".", "q2");


  AFNDInsertaLTransicion(afd, "q0", "q1");
  AFNDInsertaLTransicion(afd, "q3", "q5");
  AFNDCierraLTransicion(afd);

  return afd;
}

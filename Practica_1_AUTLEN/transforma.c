#include <stdio.h>

#include "transforma.h"


AFND * AFNDTransforma(AFND * p_afnd){
  AFND * afd;
  int i, j, simb;
  int* finales;
  int* matriz_est_dist[AFNDNumEstados(p_afnd)][AFNDNumEstados(p_afnd)] = NULL;

  afd= AFNDNuevo("afd", 6, AFNDNumSimbolos(p_afnd));

  for(i = 0;i<AFNDNumEstados(p_afnd);i++){
    printf("%s %d \t", AFNDNombreEstadoEn(p_afnd, i), AFNDTipoEstadoEn(p_afnd, i)); 

    for (j = 0; j < AFNDNumEstados(p_afnd); j++){
      printf("\n%s -->: %s\n", AFNDNombreEstadoEn(p_afnd, i), AFNDNombreEstadoEn(p_afnd, j)); 
      printf("Transiciones Lambda:"); 
      printf(" %d \t", AFNDLTransicionIJ(p_afnd, i , j));
      printf("\n");

      for(simb = 0; simb < AFNDNumSimbolos(p_afnd); simb++){
        printf("Transiciones simb %s: ", AFNDSimboloEn(p_afnd, simb));
        printf("  %d \t", AFNDTransicionIndicesEstadoiSimboloEstadof(p_afnd, i, simb, j));
      }
      printf("\n");
    }
    printf("\n");
    printf("\n");
  }

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

/*
Archivo: minimiza.c
Funcionalidad: Algoritmo y funciones de minimización de un AFD.
Autores: Alfonso Camblor y Julio Carreras
*/

#include "../include/minimiza.h"


/*
Funcion: finalesAFND
Funcionalidad: dividir los estados del autómata en finales y no finales.
Argumentos:
    AFND* afnd = automata finito determinista creado y listo para ser minimizado.
    int numEstados = numEstados del AFND.
*/

int inicial;
int* estados;
int* accesibles;
int inifinal = 0;

int *finalesAFND(AFND *afnd, int numEstados)
{   
    int i;
    estados = (int *)malloc(sizeof(int) * (AFNDNumEstados(afnd)));
    if(!estados)
        return NULL;

    accesibles = (int*)malloc(sizeof(int) * numEstados);
    if(!accesibles)
        return NULL;

    for(i = 0; i<AFNDNumEstados(afnd); i++){
        accesibles[i] = 0;
    }

    /* Bucle que divide los estados en finales y no finales, e inicializa la variable global usada para saber cual es el estado inicial*/

    /* Buscamos estado final e inicial */
    for (i = 0; i < numEstados; i++){     
        if((AFNDTipoEstadoEn(afnd, i) == 2)){
            estados[i] = 0;
            inicial = i;
            inifinal = 1;
            accesibles[i] = 1;
        }
    }
    /*Intentaremos que el conjunto de iniciales sea el 0 */
    if(inifinal == 1){
        for (i = 0; i < numEstados; i++){
            if((AFNDTipoEstadoEn(afnd, i) == 1 || AFNDTipoEstadoEn(afnd, i) == 2)){
                estados[i] = 0;
            }

            else{
                estados[i] = 1;
            }        
        }
        for(i = 0; i<AFNDNumEstados(afnd); i++){
        printf("q%d = %d, ", i, estados[i]);
    }
    }
    else{
        for (i = 0; i < numEstados; i++){
            if((AFNDTipoEstadoEn(afnd, i) == 1)){
                estados[i] = 1;
            }

            else{
                estados[i] = 0;
                if((AFNDTipoEstadoEn(afnd, i) == 0)){
                    inicial = i;
                    accesibles[i] = 1;
                }  
            }        
        }
    }
        
    return estados;
}



/*
Funcion: transiciones_est
Funcionalidad: Divide el afnd en conjuntos de estados distinguibles mediante sus transiciones y os devuelve el número de conjuntos obtenido.
Argumentos:
    AFND* afnd = automata finito determinista creado y listo para ser minimizado.
    int numEstados = numEstados del AFND.
*/

int transiciones_est(AFND *afnd, int numEstados)
{   
    int i=1, i2, i3, a, prox = 0, cont /*contador de pasos del bucle*/, conj=2/*proximo conjunto posible*/, simb/*simbolo*/, est1/*estado a comprobar*/, est2/*estado para comparar*/, cambio = 0;
    int* estados_temp;

    estados_temp = (int*)malloc(sizeof(int) * numEstados);

    /*Con este bucle obtenemos los diferentes conjuntos de estados dependiendo de sus transiciones*/
    for(i = prox, cont = 0; cont < numEstados; cont++){
        i = prox;
        /*Estados temporales para que los cambios no afecten al recorrer el array */
        for(a=0;a<numEstados;a++){
            estados_temp[a] = estados[a];
        }
        for(i2 = 0; i2 < numEstados; i2++){
            if (estados[i] == estados[i2]){
                printf ("\n q%d // q%d\n", i, i2);
                for(simb=0; simb<AFNDNumSimbolos(afnd); simb++){   
                    for(i3 = 0; i3<numEstados; i3++){         
                        if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i,simb, i3)) == 1){
                            est1 = i3;
                            accesibles[i3] = 1;
                            printf ("\nTransicion símbolo %d: %d --> %d\n", simb, i, est1);
                        }
                    }
                    for(i3 = 0; i3<numEstados; i3++){         
                        if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i2,simb, i3)) == 1){
                            est2 = i3;
                            accesibles[i3] = 1;
                            printf ("Transicion símbolo %d: %d --> %d\n\n", simb, i2, est2);
                        }
                    }
                    if (estados[est1] != estados[est2]){
                        printf ("Diferente clase \n");
                        estados_temp[i2] = -1;
                    }
                }
            }
        }
        /*Realizamos los cambios del temporal en estados*/
        for(a=0;a<numEstados;a++){
            estados[a] = estados_temp[a];
        }
        /* Sustituimos dichos cambios por el próximo conjunto de estados distinguibles disponible*/
        for(i2 = 0; i2 < numEstados; i2++){
            if(estados[i2] == -1){
                if (cambio == 0){
                    prox = i2;
                }
                cambio = 1;
                estados[i2] = conj;
                printf ("\n PROX = %d  q%d pertenece a conjunto %d ahora \n", prox, i2, conj);
            }
        }
        if(cambio == 1){
            cambio = 0;
            conj++;
        }
        else{
            prox++;
        }
    }

    free(estados_temp);
    return conj;
}



/*
Funcion: crea_afnd
Funcionalidad: Crea el Autómata finito determinista a partir de el número de conjuntos de estados distinguibles obtenidos.
Argumentos:
    AFND* afnd = automata finito determinista creado y listo para ser minimizado.
    int numEstados = numEstados del AFND.
    int conj = número de conjuntos de estados distinguibles.
*/

AFND* crea_afnd(AFND *afnd, int numEstados, int conj){
    int i, i2, i3, j = 0, simb/*simbolo*/, est1/*estado a comprobar*/, k = 1/*para nombres*/, l;
    AFND* p_afnd;
    char** nombres = NULL; 
    int tipo;

    p_afnd = AFNDNuevo("afmin",conj,AFNDNumSimbolos(afnd));


    nombres = malloc(conj * sizeof(char*));


    /*Insertamos simbolos en nuestro afnd*/
    for(simb = 0; simb<AFNDNumSimbolos(afnd); simb++){
        AFNDInsertaSimbolo(p_afnd, AFNDSimboloEn(afnd, simb));
    }

    /*Para nuestro estado inicial*/
    nombres[0] = malloc((80+1) * sizeof(char)); 
    if (!nombres[0]){
        return NULL;
    }        
    nombres[0][0] = '\0';

    for(j=0;j<numEstados;j++){
        if(estados[j] == estados[inicial]){
            strcat(nombres[0], AFNDNombreEstadoEn(afnd, j));
        }
    }
    printf ("\n%s\n", nombres[0]);
    if (inifinal == 1){
        AFNDInsertaEstado(p_afnd, nombres[0], INICIAL_Y_FINAL);
    }
    else {
        AFNDInsertaEstado(p_afnd, nombres[0], INICIAL);
    }

    /*Creamos el resto de estados*/
    for(i=0; i < conj; i++){
        if(i!= estados[inicial]){
            nombres[k] = malloc((80+1) * sizeof(char));
            if (!nombres[k]){
                return NULL;
            }        
            nombres[k][0] = '\0';

            for(j=0;j<numEstados;j++){
                if(estados[j] == i){
                    tipo = AFNDTipoEstadoEn(afnd, j);
                    if(accesibles[j] != 0){
                        strcat(nombres[k], AFNDNombreEstadoEn(afnd, j));
                    }                
                }
            }
            printf ("\n%s\n", nombres[k]);
            AFNDInsertaEstado(p_afnd, nombres[k], tipo);
            k++;
        }
    }

    /*Insertamos las transiciones inicial*/
    for(simb=0; simb<AFNDNumSimbolos(afnd); simb++){   
        for(i3 = 0; i3<numEstados; i3++){         
            if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, inicial,simb, i3)) == 1){
                est1 = i3;
                AFNDInsertaTransicion(p_afnd, AFNDNombreEstadoEn(p_afnd, 0), AFNDSimboloEn(p_afnd, simb), AFNDNombreEstadoEn(p_afnd, estados[est1]));

            }        
        }
    }
    
    /*Insertamos las transiciones del resto de estados*/
    for(i=0; i < conj; i++){
        if(i!= estados[inicial]){
            for(l = 0; l<numEstados;l++){
                if (estados[l] == i){
                    i2 = l;
                }
            }
            for(simb=0; simb<AFNDNumSimbolos(afnd); simb++){   
                for(i3 = 0; i3<numEstados; i3++){         
                    if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i2,simb, i3)) == 1){
                        est1 = i3;
                        AFNDInsertaTransicion(p_afnd, AFNDNombreEstadoEn(p_afnd, estados[i2]), AFNDSimboloEn(p_afnd, simb), AFNDNombreEstadoEn(p_afnd, estados[est1]));
                    }
                }
            }
        }
    }
    
    for(i=0;i<conj;i++){
        free(nombres[i]);
    }
    
    free(nombres);
    return p_afnd;
}



AFND * AFNDMinimiza(AFND * afnd){
    int i, conjuntos;
    AFND* p_afnd;

    estados = finalesAFND(afnd, AFNDNumEstados(afnd));
    conjuntos = transiciones_est(afnd, AFNDNumEstados(afnd));
    p_afnd = crea_afnd(afnd, AFNDNumEstados(afnd), conjuntos);

    for(i = 0; i<AFNDNumEstados(afnd); i++){
        printf("q%d = %d, ", i, estados[i]);
    }

    printf ("\n\n");
    for(i = 0; i<AFNDNumEstados(afnd); i++){
        printf("q%d = %d, ", i, accesibles[i]);
    }


    free(accesibles);
    free(estados);
    return p_afnd;
}

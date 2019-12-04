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

int *finalesAFND(AFND *afnd, int numEstados)
{   
    int i;
    int *estados;
    estados = (int *)malloc(sizeof(int) * (AFNDNumEstados(afnd)));
    if(!estados)
        return NULL;

    /* Bucle que divide los estados en finales y no finales, e inicializa la variable global usada para saber cual es el estado inicial*/
    for (i = 0; i < numEstados; i++)
    {         
        if((AFNDTipoEstadoEn(afnd, i) == 2)){
            estados[i] = 0;
            inicial = i;
        }
        else if((AFNDTipoEstadoEn(afnd, i) == 1)){
            estados[i] = 0;
        }

        else{
            estados[i] = 1;
            if((AFNDTipoEstadoEn(afnd, i) == 0)){
                inicial = i;
            }  
        }

        
    }
        
    return estados;
}

int **matriz_finales(AFND *afnd, int numEstados, int* estados)
{   
    int i, i2, i3, j = 0, k = 0, conj=2/*proximo conjunto posible*/, simb/*simbolo*/, est1/*estado a comprobar*/, est2/*estado para comparar*/, cambio = 0;
    int *finales;
    int *nofinales;
    

    finales = (int*)malloc(sizeof(int));
    nofinales = (int*)malloc(sizeof(int));


    for (i = 0; i<numEstados; i++){
        if(estados[i] == 0){
            finales[j] = i;
            j++;
            finales = realloc(finales, (j * sizeof(int)));            
        }

        if(estados[i] == 1){
            nofinales[k] = i;
            k++;
            nofinales = realloc(nofinales, (k * sizeof(int)) + sizeof(int));            
        }
    }
    printf("\nfinales:\n");
    for(i=0; i<j; i++){
        printf("q%d,  ",  finales[i]);
    }

    printf("Size: %d,  ", sizeof(finales));

    printf("\nno finales:\n");
    for(i=0; i<k; i++){
        printf("q%d,  ",  nofinales[i]);
    }


    /*Bucle para finales*/
    for(i = 0; i<j; i++){
        for(i2 = i+1; i2<j; i2++){
            for(simb=0; simb<AFNDNumSimbolos(afnd); simb++){   
                for(i3 = 0; i3<numEstados; i3++){         
                    if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, finales[i],simb, i3)) == 1){
                      est1 = i3;
                      printf ("\nTransicion símbolo %d: %d --> %d\n", simb, finales[i], est1);
                    }
                }
                for(i3 = 0; i3<numEstados; i3++){         
                    if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, finales[i2],simb, i3)) == 1){
                      est2 = i3;
                      printf ("Transicion símbolo %d: %d --> %d\n", simb, finales[i2], est2);
                    }
                }
                if (estados[est1] != estados[est2]){
                    cambio = 1;
                }
            }
            if (cambio == 1){
                    printf ("\n Diferente clase \n");
                    estados[finales[i2]] = conj;
                    printf ("\n q%d pertenece a conjunto %d ahora \n", finales[i2], conj);
                    conj++;
            }
            else{
                printf ("\n %d y %d son de la Misma clase \n", finales[i], finales[i2]);
                if (estados[finales[i2]] != estados[finales[i]]){
                    estados[finales[i2]] = estados[finales[i]];
                    conj--;
                }
            }  
        }
    }

    /*Bucle para no finales*/

    for(i = 0; i<k; i++){
        for(i2 = i+1; i2<k; i2++){
            for(simb=0; simb<AFNDNumSimbolos(afnd); simb++){   
                for(i3 = 0; i3<numEstados; i3++){         
                    if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, nofinales[i],simb, i3)) == 1){
                      est1 = i3;
                      printf ("\nTransicion símbolo %d: %d --> %d\n", simb, nofinales[i], est1);
                    }
                }
                for(i3 = 0; i3<numEstados; i3++){         
                    if ((AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, nofinales[i2],simb, i3)) == 1){
                      est2 = i3;
                      printf ("Transicion símbolo %d: %d --> %d\n", simb, nofinales[i2], est2);
                    }
                }
                if (estados[est1] != estados[est2]){
                    printf ("[q%d] = %d, [q%d] = %d\n", est1, estados[est1], est2, estados[est2]);
                    cambio = 1;
                }
            }
            if (cambio == 1){
                printf ("\n Diferente clase \n");
                estados[nofinales[i2]] = conj;
                printf ("\n q%d pertenece a conjunto %d ahora \n", nofinales[i2], conj);
                conj++;
                cambio = 0;
            }
            else{
                printf ("\n %d y %d son de la Misma clase \n", nofinales[i], nofinales[i2]);
                if (estados[nofinales[i2]] != estados[nofinales[i]]){
                    estados[nofinales[i2]] = estados[nofinales[i]];
                    conj--;
                }
            }  
        }
    }

    for(i=0; i<k; i++){
        for(simb=0; simb<AFNDNumSimbolos(afnd); simb++){

        }
    }
    
    printf("\n\n");

    return NULL;
}


AFND * AFNDMinimiza(AFND * afnd){
    int i;

    estados = finalesAFND(afnd, AFNDNumEstados(afnd));
    matriz_finales(afnd, AFNDNumEstados(afnd), estados);

    for(i = 0; i<AFNDNumEstados(afnd); i++){
        printf("q%d = %d, ", i, estados[i]);
    }

    return afnd;
}

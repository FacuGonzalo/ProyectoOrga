
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errores.h"
#include "pila.h"


//inicializa la pila, reserva espacio en memoria.
pila_t pila_crear(){
    pila_t nueva = malloc(sizeof(struct pila));
    nueva->c= NULL;
    nueva->proximo_elemento=NULL;

    return nueva;
}

//Devuelve 0 si la pila esta vacia, 1 en caso contrario. Error si
//la pila no estaba inicializada en la invocacion.
int pila_vacia(pila_t pila){
    if (pila==NULL)
        exit(PIL_NO_INI);
    else
        if ((pila->c) ==NULL)
            return 0;
        else
            return 1;
}

//asigna en el tope de la pila el nuevo elemento, devuelve cero si hay memoria disponible
// y se logro el apilado, uno en caso contrario. Error si pila no esta inicializada.
int apilar(pila_t *pila, char *str){
    pila_vacia(*pila);//verifica si la pila esta inicializada
    pila_t nodo= malloc(sizeof(struct pila));
        if (nodo==NULL)//no hay memoria pues malloc dio nulo.
            return 1;
    nodo->c = str;
    nodo->proximo_elemento= *pila;
    *pila= nodo;

    return 0;
}

//Devuelve el elemento del tope de la pila, error si la pila esta vacia o no inicializada.
char* tope(pila_t pila){
    if (pila_vacia(pila)==0)
        exit(PIL_VACIA);
    return (pila->c);
}

//Elimina el elemento del tope de la pila y lo retorna, error si la pila esta
//vacia o no se encuentra inicializada.
char* desapilar(pila_t *pila){
    if (pila_vacia(*pila)==0)
        exit(PIL_VACIA);
    char *res= ((*pila)->c);
    pila_t aux= *pila;
    free(*pila);
    *pila= (aux)->proximo_elemento;
    return res;
}



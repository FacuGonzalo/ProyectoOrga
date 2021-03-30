
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errores.h"
#include "lista.h"


//Crea y reserva espacio para una nueva lista de elementos, y la retorna.
lista_t lista_crear(){
    lista_t nueva= malloc(sizeof(struct lista_eficiente));
    celda_t *celda= malloc(sizeof(struct celda));
    int i=0;
    for( ;i<4;i++)
        celda->elementos[i]=-1;

    celda->proxima_celda=NULL;
    nueva->primera_celda= celda;
    nueva->cantidad_elementos=0;
    return nueva;
}


//Devuelve la cantidad de elementos de la lista, error si la lista no esta inicializada.
int lista_cantidad(lista_t lista){
    if (lista==NULL)
        exit(LST_NO_INI);
    else
        return lista->cantidad_elementos;
}


//Inserta elem en lista, en la posicion recibida como parametro pos y devuelve cero.
//Si la posicion recibida es invalida retorna uno.
int lista_insertar(lista_t lista, unsigned int pos, int elem){
    if (pos>lista_cantidad(lista))//posicion incorrecta
        return 1;
    int i,posicion=0;
    celda_t* ultima=NULL;
    celda_t* celdaInsertar= lista->primera_celda;

    if (pos==lista_cantidad(lista)) {
        lista->cantidad_elementos++;
        if (((pos%4)==0) && pos!=0){
            //creo nueva celda si la lista no esta vacia.
            ultima=lista->primera_celda;
            celda_t *celda= malloc(sizeof(celda_t));

            for( ;i<4;i++)
                celda->elementos[i]= -1;
            //asigno nueva celda al final de la lista
            celda->proxima_celda= NULL;
            while (ultima->proxima_celda != NULL)
                ultima= ultima->proxima_celda;
            ultima->proxima_celda= celda;
        }
    }
    if (ultima!=NULL)
        celdaInsertar= ultima->proxima_celda;
    else{
        i=0;
        posicion= pos/4;//nro de celda
        for(; i<posicion;i++)
            celdaInsertar= celdaInsertar->proxima_celda;
    }
    celdaInsertar->elementos[pos%4]=elem;
    return 0;
}



//Devuelve el elemento de lista que se encuentra en la posicion pos.
//Error si la posicion recibida es invalida.
int lista_obtener(lista_t lista, unsigned int pos){
    if (pos>=lista_cantidad(lista))
        exit(LST_POS_INV);
    int i=0;
    int posicion= pos/4;
    celda_t *celdaBuscar=lista->primera_celda;

    for( ; i<posicion;i++)
        celdaBuscar= celdaBuscar->proxima_celda;

    return (celdaBuscar->elementos[pos%4]);
}


//Metodo privado: reacomoda los elementos linealmente desde pos hasta el ultimo
//correspondiente a la lista, de izquierda a derecha respectivamente.
void reacomodarLista(lista_t lista, int pos){
    int i=0;
    int posicion= pos/4;
    int posicion2,elemento1,elemento2=0;
    celda_t *celda= lista->primera_celda;
    //localizo la celda
    for( ;i<posicion; i++)
        celda= celda->proxima_celda;
    //voy intercambiando elementos
    posicion=pos;
    posicion2= pos+1;

    elemento1=lista_obtener(lista,posicion);
    elemento2=lista_obtener(lista,posicion2);

    while(elemento2 !=-1){
        lista_insertar(lista,posicion,elemento2);
        lista_insertar(lista,posicion2,elemento1);
        posicion++;
        posicion2++;
        elemento1= lista_obtener(lista,posicion);

        if (posicion2<lista_cantidad(lista))
            elemento2= lista_obtener(lista,posicion2);
        else
            elemento2=-1;
    }
    //eliminar celda siguiente si es que esta vacia
    if (celda->proxima_celda->elementos[0] == -1)
        free(celda->proxima_celda);
 }

//elimina de lista el elemento en la posicion pos recibida y reacomoda en caso de
// ser necesario. Error si pos es invalido o la lista no esta inicializada.
int lista_eliminar(lista_t lista, unsigned int pos){
    //incluye lista vacia.....
    if (pos>=lista_cantidad(lista))
        exit(LST_POS_INV);

    celda_t *celdaEliminar=lista->primera_celda;
    int i=0;
    int posicion= pos/4;
    for( ; i<posicion;i++)
        celdaEliminar= celdaEliminar->proxima_celda;
    celdaEliminar->elementos[pos%4]= (-1);

    if (pos+1< lista_cantidad(lista))
        reacomodarLista(lista,pos);//metodo PRIVADO, elimino ahi celda vacia

    lista->cantidad_elementos--;
    return 0;
}



//agrega un nuevo elemento al final de la lista y devuelve cero.
// Retorna uno en caso contrario. Error si lista no esta inicializada.
int lista_adjuntar(lista_t lista, int elem){
    if (lista==NULL)
        exit(LST_NO_INI);
    int resultado= lista_insertar(lista,lista_cantidad(lista),elem);
    return resultado;//cero si es correcto, sino uno.
}



//metodo privado: libera los espacios de memoria reservados por la lista.
void destruirAux(lista_t lista,celda_t* celda){
    if (celda!=NULL){
        destruirAux(lista,celda->proxima_celda);
        if (celda!= (lista)->primera_celda){
            celda->proxima_celda=NULL;
            free(celda);
        }
    }
}


//elimina la lista recibida como parametro y libera el espacio en memoria, devuelve cero.
// Error si la lista no esta inicializada.
int lista_destruir(lista_t *lista){
    if (*lista==NULL)
        exit(LST_NO_INI);

    destruirAux(*lista,(*lista)->primera_celda);
    free(*lista);
    *lista=NULL;
    return 0;
}


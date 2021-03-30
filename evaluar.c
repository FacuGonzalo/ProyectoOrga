#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "pila.h"
#include "lista.h"
#include "errores.h"

//realiza la suma entre multiples operandos de la lista recibida.
//Error si la cantidad de operandos es menor a dos.
int suma(lista_t operandos){
    int num=0;
    int resultado= 0;
    resultado=0;
    int cantidad= lista_cantidad(operandos);
    if (cantidad<2)
        exit(OPND_INSUF);

    while(cantidad>0){
        num= lista_obtener(operandos, cantidad-1);
        resultado+=num;
        lista_eliminar(operandos, cantidad-1);
        cantidad--;
    }
    return resultado;
}
//realiza el producto entre multiples operandos de la lista recibida.
//Error si la cantidad de operandos es menor a dos.
int producto(lista_t operandos){
    int num,resultado=1;
    int cantidad= lista_cantidad(operandos);
    if (cantidad<2)
        exit(OPND_INSUF);

    while(cantidad>0){
        num= lista_obtener(operandos, cantidad-1);
        resultado*=num;
        lista_eliminar(operandos, cantidad-1);
        cantidad--;
    }
    return resultado;
}
//realiza la resta entre los dos operandos de la lista recibida.
//Error si la cantidad de operandos es distinta de dos, o si el resultado es negativo.
int resta(lista_t operandos){
    int cantidad= lista_cantidad(operandos);
    if (cantidad<2)
        exit(OPND_INSUF);
    else
        if (cantidad>2)
            exit(OPND_DEMAS);

    int sustraendo= lista_obtener(operandos, 0);
    int minuendo= lista_obtener(operandos, 1);
    lista_eliminar(operandos,1);
    lista_eliminar(operandos,0);
    int resultado=0;
     resultado= minuendo-sustraendo;

    if (resultado>=0)
        return resultado;
    else
        exit(OPND_INV);
}

//realiza la division entre los dos operandos de la lista recibida.
//Error si la cantidad de operandos es distinta de dos, o si el divisor es cero.
int division(lista_t operandos){
    int cantidad= lista_cantidad(operandos);
    if (cantidad<2)
        exit(OPND_INSUF);
    else
        if (cantidad>2)
            exit(OPND_DEMAS);

    int divisor= lista_obtener(operandos, 0);
    int dividendo= lista_obtener(operandos, 1);
    lista_eliminar(operandos,1);
    lista_eliminar(operandos,0);
    if (divisor==0)
        exit(OPND_INV);
    int resultado=1;
    resultado=dividendo/divisor;
    return resultado;
}

//devuelve cero si el string recibido es un operando valido, uno en caso contrario.
int esOperando(char *c){
    int i=0;
    char elem= c[i];//primer elemento
    int seguir=0;

    while (elem!='\0' && seguir==0){
        if (((int)elem>47) && ((int)elem<58)){//digito del 0 al 9 en ascii
            i++;
            elem= c[i];
        }
        else
            seguir=1;
    }
    return seguir;
}

//Captura la expresion en pre-orden, la guarda en un arreglo para facilitar su manejo,
// realiza los calculos correspondientes y devuelve un valor entero.
int evaluar(){
    char *exp= malloc(sizeof(char)*70);
    fgets(exp,70,stdin);// guardo la expresion en pre-orden.

    pila_t pila= pila_crear();
    lista_t lista= lista_crear();
    char *c;
    char *string;
    char *parcial;
    int i, posAux, resultado=0;
    //transformamos la expresion ingresada, en un arreglo con elementos que son los elementos
    // de exp que se encuentran separados por espacios.
    char** datos= malloc(sizeof(char*)*70);
    for(c=strtok(exp," \n");c;c= strtok(NULL," \n")){
        i=0;
        string = malloc(sizeof(char)*20);
        while(c[i]!='\0'){
            string[i]=c[i];
            i++;
        }
        string[i]='\0';
        datos[posAux]=string;//guardo el string sin espacios en mi arreglo
        posAux++;
    }
    datos[posAux]= NULL;//final arreglo datos con NULL
    //caso en donde ingreso un solo elemento como expresion
    if (posAux==1)
        exit(EXP_MALF);
    posAux=0;
    int entero=0;
    c= datos[posAux];
    //usamos el arreglo de datos.
    while (c!=NULL){
        if (c[0]!=')' && c[1]=='\0')//mientras el caracter sea distinto de )
            apilar(&pila,c);
        else{
            c= tope(pila);
            while (esOperando(c)==0){

                c=desapilar(&pila);
                entero= atoi(c);
                free(c);//libero el puntero para asignarle el nuevo valor.
                lista_insertar(lista,lista_cantidad(lista),entero);
                c= tope(pila);
            }
            //desapilar operador
            c= desapilar(&pila);
            //caso de que la pila tenga un solo elemento, y sea un operando valido.
            if (c[0]=='(' && pila_vacia(pila)==0){
                printf("\n->El resultado de la expresion es: %i \n\n",lista_obtener(lista,0));
                lista_eliminar(lista,0);
                exit(EXITO);
            }
            //realizar cuenta
            if (strcmp(c,"+")==0)
                resultado= suma(lista);
            else
                 if (strcmp(c,"*")==0)
                    resultado= producto(lista);
                else
                    if (strcmp(c,"-")==0)
                        resultado= resta(lista);
                    else
                        if (strcmp(c,"/")==0)
                            resultado= division(lista);
                        else//operador no valido o ( ) desbalanceado
                            if ((strcmp(c,"(")==0) || (strcmp(c,")")==0))
                                exit(EXP_MALF);
                            else//error: otro simbolo
                                exit(OPRD_INV);
        //malloc para el nuevo tope de la pila
        parcial = malloc(sizeof(char)*4);

        //consumir y verificar parentesis (
        c= desapilar(&pila);
        if (c[0]!='(' && c[1]!='\0')
            exit(EXP_MALF);
        //de entero a string
        sprintf(parcial,"%i",resultado);
        apilar(&pila, parcial);
        }//fin else

        posAux++;
        c= datos[posAux];//siguiente elemento del arreglo.
    }//fin while
    resultado= atoi(desapilar(&pila));
    //compruebo que la pila este vacia al terminar el calculo.
    if (pila_vacia(pila)==1)
        exit(EXP_MALF);

    //libero las locaciones reservadas
    i=0;
    while(datos[i]!=NULL){
        free(datos[i]);
        i++;
    }
    free(datos);
    free(exp);
    free(parcial);
    free(pila);
    lista_destruir(&lista);

    return resultado;
}


void imprimirAyuda(){
    printf("\nAYUDA para evaluar: Este programa evalua y calcula una expresion ");
    printf("escrita en pre-orden\n__los operandos y resultados validos son numeros \n");
    printf("mayores o iguales a cero.\n __Las operaciones posibles son +(suma) -(resta) /(mult) *(div)\n");
    printf("__Todo debe estar correctamente parentizado y separado por al menos ");
    printf("un espacio entre elementos de la expresion. Ej: ( * ( + 2 3 ) 2 3 ) \n");
    printf("Y el resultado sera 30 \n");
    printf("Operaciones + y * permiten multiples operandos, en cambio - y / solo dos.\n");
}

int main(int argc, char** argv){

    if (argc>1){
        if (strcmp(argv[1],"-h")==0){
            imprimirAyuda();
            exit(0);
        }
        else{
            printf("Parametro ingresado desconocido %s",argv[1]);
            imprimirAyuda();
            exit(1);
        }
    }
    else{
        printf("Ingrese expresion en preorden abarcada entre (  ) :\n");

        int resultado= evaluar();
        printf("\n->El resultado de la expresion es: %i \n\n",resultado);
    }
  return 0;
}


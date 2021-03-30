#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

//struct pila;
typedef struct pila{
 char* c;
 struct pila* proximo_elemento;

} *pila_t;

pila_t pila_crear();
char* tope(pila_t pila);
char* desapilar(pila_t *pila);
int apilar(pila_t* pila, char* str);
int pila_vacia(pila_t pila);

#endif // PILA_H_INCLUDED

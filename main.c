#include <stdio.h>
#include "rbtree.h"
#include "rb_tests.h"
#include <sys/time.h>
#include <stdlib.h>


int main() {
    /*
    Prueba miles de arboles, agregando y borrando nodos
    al azar, y en cada paso viendo si el arbol es válido
    y se cumplió lo que se quería hacer.
    
    stress_test();   

    */
   /* rottotal = 0;    */
    int elementos,
        raiz;
    rb_arbol* arbol;

    srand ( time(NULL) );        
    elementos = 500;
    raiz = rand() % 500;
    arbol = rb_nuevo_arbol(raiz);
    while(--elementos) {
        rb_insertar(arbol, rand() % 500);   
    }
    rb_dump_arbol(arbol);
    rb_destruir_arbol(arbol);
    return 0;   
}

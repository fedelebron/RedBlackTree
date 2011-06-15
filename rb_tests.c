#include "rbtree.h"
#include "rb_tests.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>


bool checkear_conteo(rb_arbol* arbol, rb_nodo* r, int n, int i) {
    if(r == arbol->nil) {
        return i == n;
    }
    if(rb_color(r) == ROJO) {
        return (checkear_conteo(arbol, rb_izquierdo(r), n, i) && checkear_conteo(arbol, rb_derecho(r), n, i));
    } else {    
        return (checkear_conteo(arbol, rb_izquierdo(r), n, i+1) && checkear_conteo(arbol, rb_derecho(r), n, i+1));
    }
}

bool checkear_rojos(rb_arbol* arbol, rb_nodo* r) {
    if(r == arbol->nil) {
        return true;
    }
    if(rb_color(r) == ROJO) {
        if(rb_color(rb_izquierdo(r)) == ROJO || rb_color(rb_derecho(r)) == ROJO) {
            return false;
        }
    }
    return (checkear_rojos(arbol, rb_izquierdo(r)) && checkear_rojos(arbol, rb_derecho(r)));
}

bool check(rb_arbol* arbol) {
    int n;
    rb_nodo* nodo;
    
    n = 1;
    nodo = arbol->raiz;
    if(rb_es_hoja(arbol, nodo)) {
        return rb_color(nodo) == NEGRO;
    }
    while(rb_izquierdo(nodo) != arbol->nil) {
        nodo = rb_izquierdo(nodo);
        if(rb_color(nodo) == NEGRO) {
            n++;
        }
    }
    
    return rb_color(arbol->raiz) == NEGRO && checkear_conteo(arbol, arbol->raiz, n, 0) && checkear_rojos(arbol, arbol->raiz);    
}

void stress_test() {
    int limit = 50,
        root,
        total = 1000,
        n;
    rb_arbol* arbol;
    
    root = rand() % limit;
    srand ( time(NULL) );
    arbol = rb_nuevo_arbol(root);
    while(--total) {
        n = rand() % limit;
        printf("---OLD---\n");
        rb_dump_arbol(arbol);
        if(rb_buscar(arbol, n) != arbol->nil) {
            rb_eliminar(arbol, n);
            if(rb_buscar(arbol, n) != arbol->nil) {
                printf("No elimine.\n");
                exit(1);
            }
        } else {
            rb_insertar(arbol, n);
            if(rb_buscar(arbol, n) == arbol->nil) {
                printf("No inserte.\n");
                exit(1);
            }
        }
        printf("---/OLD---\n");
        if(!check(arbol)) {
            printf("ERROR: INCONSISTENCIA con %d.\n", n);
            rb_dump_arbol(arbol);
            printf("ERROR: INCONSISTENCIA con %d.\n", n);

            exit(1);
        }
    }
    rb_destruir_arbol(arbol);
}

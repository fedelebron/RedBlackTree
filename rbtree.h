#include <stdio.h>
#include <stdlib.h>

typedef enum rb_color_ {
    NEGRO,
    ROJO
} rb_color_nodo;

/* solo para claridad */
typedef enum boolean {
    false = 0, 
    true = 1
} bool;

typedef enum rotacion_ {
    DERECHA,
    IZQUIERDA
} rotacion;

typedef struct rb_nodo_ {
    struct rb_nodo_* hijos[2];
    struct rb_nodo_* padre;
    rb_color_nodo color;
    int datos;
} rb_nodo;

typedef struct rb_arbol_ {
    rb_nodo* raiz;
    rb_nodo* nil;
} rb_arbol;


rb_nodo* rb_set_derecho(rb_arbol*, rb_nodo*, rb_nodo*);
rb_nodo* rb_derecho(rb_nodo*) __attribute__((always_inline));
rb_nodo* rb_set_izquierdo(rb_arbol*, rb_nodo*, rb_nodo*);
rb_nodo* rb_izquierdo(rb_nodo*) __attribute__((always_inline));
unsigned int rb_contar_hijos(rb_arbol*, rb_nodo*);
bool rb_es_hoja(rb_arbol*, rb_nodo*);
rb_color_nodo rb_color(rb_nodo*) __attribute__((always_inline));
rb_nodo* rb_padre(rb_nodo*) __attribute__((always_inline));
rb_nodo* rb_set_padre(rb_nodo*, rb_nodo*);
void rb_eliminar_hijo(rb_arbol*, rb_nodo*, rb_nodo*);
rb_color_nodo rb_set_color(rb_nodo*, rb_color_nodo);
int rb_datos(rb_nodo*) __attribute__((always_inline));
int rb_set_datos(rb_nodo*, int);
void rb_copiar(rb_arbol*, rb_nodo*, rb_nodo*);
void rb_swap(rb_arbol*, rb_nodo*, rb_nodo*);
rb_nodo* rb_hermano(rb_nodo*);
rb_nodo* rb_arbol_raiz(rb_arbol*);
void rb_arbol_set_raiz(rb_arbol*, rb_nodo*);
void rb_dump(rb_arbol*, rb_nodo*, int, int);
void rb_dump_arbol(rb_arbol*);

rb_nodo* rb_nuevo(rb_arbol*, rb_nodo*, rb_nodo*, rb_color_nodo, int);
rb_arbol* rb_nuevo_arbol(int);
void rb_destruir_arbol(rb_arbol*);
void rb_destruir_nodo(rb_arbol*, rb_nodo*);


rb_nodo* rb_buscar_nodo(rb_arbol*, int, rb_nodo*);
rb_nodo* rb_buscar(rb_arbol*, int);
void rb_rotar_derecha(rb_arbol*, rb_nodo*);
void rb_rotar_izquierda(rb_arbol*, rb_nodo*);

rb_nodo* rb_maximo(rb_arbol*);
rb_nodo* rb_minimo(rb_arbol*);
rb_nodo* rb_maximo_nodo(rb_arbol*, rb_nodo*);
rb_nodo* rb_minimo_nodo(rb_arbol*, rb_nodo*);
rb_nodo* rb_sucesor(rb_arbol*, rb_nodo*);
rb_nodo* rb_predecesor(rb_arbol*, rb_nodo*);

int rb_insertar(rb_arbol*, int);
int rb_insertar_nodo(rb_arbol*, rb_nodo*, int);

void rb_arreglar_insercion(rb_arbol*, rb_nodo*);

void rb_eliminar(rb_arbol*, int);
void rb_eliminar_nodo(rb_arbol*, rb_nodo*);
void rb_arreglar_eliminacion(rb_arbol*, rb_nodo*, rb_nodo*);

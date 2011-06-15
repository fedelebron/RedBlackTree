#include "rbtree.h"


rb_nodo* rb_set_izquierdo(rb_arbol* arbol, rb_nodo* nodo, rb_nodo* izq) {
    nodo->hijos[0] = izq;
    if(izq != arbol->nil) {
        rb_set_padre(izq, nodo);
    }
    return izq;
}

rb_nodo* rb_izquierdo(rb_nodo* nodo)  {
    return (nodo->hijos)[0];
}

rb_nodo* rb_set_derecho(rb_arbol* arbol, rb_nodo* nodo, rb_nodo* der) {
    nodo->hijos[1] = der;
    if(der != arbol->nil) {
        rb_set_padre(der, nodo);
    }
    return der;
}

rb_nodo* rb_derecho(rb_nodo* nodo)  {
    return (nodo->hijos)[1];
}

unsigned int rb_contar_hijos(rb_arbol* arbol, rb_nodo* nodo) {
    int ret = 0;
    if(rb_izquierdo(nodo) != arbol->nil) {
        ++ret;
    }
    if(rb_derecho(nodo) != arbol->nil) {
        ++ret;
    }
    return ret;
}

bool rb_es_hoja(rb_arbol* arbol, rb_nodo* nodo) {
    return rb_contar_hijos(arbol, nodo) == 0;
}

rb_color_nodo rb_set_color(rb_nodo* nodo, rb_color_nodo color) {
    nodo->color = color;
    return color;
}
rb_color_nodo rb_color(rb_nodo* nodo)  {
    return nodo->color;
}

int rb_set_datos(rb_nodo* nodo, int datos) {
    nodo->datos = datos;
    return datos;
}
int rb_datos(rb_nodo* nodo) {
    return nodo->datos;
}

rb_nodo* rb_padre(rb_nodo* nodo) {
    return nodo->padre;
}

rb_nodo* rb_set_padre(rb_nodo* nodo, rb_nodo* padre) {
    nodo->padre = padre;
    return padre;
}

void rb_eliminar_hijo(rb_arbol* arbol, rb_nodo* padre, rb_nodo* hijo) {
    if(padre == arbol->nil) {
        free(hijo);
    } else {
        if(rb_izquierdo(padre) == hijo) {
            free(hijo);
            rb_set_izquierdo(arbol, padre, arbol->nil);
        } else {
            free(hijo);
            rb_set_derecho(arbol, padre, arbol->nil);    
        }
    }
}

rb_nodo* rb_hermano(rb_nodo* nodo) {
    /* asumo que no es el raiz */
    if(rb_izquierdo(rb_padre(nodo)) == nodo) {
        return rb_derecho(rb_padre(nodo));
    }
    return rb_izquierdo(rb_padre(nodo));
}

void rb_copiar(rb_arbol* arbol, rb_nodo* x, rb_nodo* y) {
    /* copio todo lo de y, y lo pongo en x */

    rb_set_derecho(arbol, x, rb_derecho(y));
    rb_set_izquierdo(arbol, x, rb_izquierdo(y));
    rb_set_color(x, rb_color(y));
    rb_set_datos(x, rb_datos(y));
    rb_set_padre(x, rb_padre(y)); 
}

void rb_swap(rb_arbol* arbol, rb_nodo* a, rb_nodo* b) {
    int dat;
    dat = rb_datos(a);
    rb_set_datos(a, rb_datos(b));
    rb_set_datos(b, dat);
}

rb_nodo* rb_arbol_raiz(rb_arbol* arbol) {
    return arbol->raiz;
}

void rb_arbol_set_raiz(rb_arbol* arbol, rb_nodo* raiz) {
    arbol->raiz = raiz;
    raiz->padre = arbol->nil; /* importante mantener este invariante */
}

void rb_dump(rb_arbol* arbol, rb_nodo* nodo, int nivel, int indice) {
    if(nivel == 0) {
        printf("digraph g%d {\n", indice);
        printf("node [height=.1, style=filled];\n");
    }
    if(nodo == arbol->nil) {
        /* caso en el que solo esta el nil */
        printf("null_%d[ width=.3, fontsize=7, label=\"NIL\", color=black, fontcolor=white, shape=record ];\n", indice);
        return;
    }
    printf("node%d_%d[ label=\"%d\", color=black, fillcolor=%s, fontcolor=%s];\n", indice, rb_datos(nodo), rb_datos(nodo), (rb_color(nodo) == ROJO) ? "red" : "black", (rb_color(nodo) == ROJO) ? "black": "white");
    if(rb_izquierdo(nodo) != arbol->nil) {
        rb_dump(arbol, rb_izquierdo(nodo), 1, indice);
        printf("node%d_%d -> node%d_%d;\n", indice, rb_datos(nodo), indice, rb_datos(rb_izquierdo(nodo)));
    } else {
        printf("null_i_%d_%d[ width=.3, fontsize=7, label=\"NIL\", color=black, fontcolor=white, shape=record ];\n", indice, rb_datos(nodo)); 
        printf("node%d_%d -> null_i_%d_%d;\n", indice, rb_datos(nodo), indice, rb_datos(nodo));
    }   
    
    if(rb_derecho(nodo) != arbol->nil) {
        rb_dump(arbol, rb_derecho(nodo), 1, indice);
        printf("node%d_%d -> node%d_%d;\n", indice, rb_datos(nodo), indice, rb_datos(rb_derecho(nodo)));
    } else {
        printf("null_d_%d_%d[ width=.3, fontsize=7, label=\"NIL\", color=black, fontcolor=white, shape=record ];\n", indice, rb_datos(nodo)); 
        printf("node%d_%d -> null_d_%d_%d;\n", indice, rb_datos(nodo), indice, rb_datos(nodo));
    }   

    
    if(nivel == 0) {
        printf("}\n");
    }
}

void rb_dump_arbol(rb_arbol* arbol) {
    rb_dump(arbol, arbol->raiz, 0, 0);
}

rb_nodo* rb_nuevo(rb_arbol* arbol, rb_nodo* izquierda, rb_nodo* derecha, rb_color_nodo color, int valor) {
    rb_nodo* nuevo = malloc(sizeof(rb_nodo));
    rb_set_izquierdo(arbol, nuevo, izquierda);
    rb_set_derecho(arbol, nuevo, derecha);
    rb_set_color(nuevo, color);
    rb_set_datos(nuevo, valor);
    rb_set_padre(nuevo, arbol->nil);
    return nuevo;
}

rb_arbol* rb_nuevo_arbol(int valor) {
    rb_arbol* nuevo;
    rb_nodo* raiz;
    
    nuevo = malloc(sizeof(rb_arbol));
    nuevo->nil = malloc(sizeof(rb_nodo));
    rb_set_izquierdo(nuevo, nuevo->nil, nuevo->nil);
    rb_set_derecho(nuevo, nuevo->nil, nuevo->nil);
    rb_set_color(nuevo->nil, NEGRO);
    rb_set_datos(nuevo->nil, 0);
    
    raiz = rb_nuevo(nuevo, nuevo->nil, nuevo->nil, NEGRO, valor);
    nuevo->raiz = raiz;
    return nuevo;
}

void rb_destruir_arbol(rb_arbol* arbol) {
    rb_destruir_nodo(arbol, arbol->raiz);
    free(arbol->nil);
    free(arbol);
}

void rb_destruir_nodo(rb_arbol* arbol, rb_nodo* nodo) {
    if(rb_izquierdo(nodo) != arbol->nil) {
        rb_destruir_nodo(arbol, rb_izquierdo(nodo));
    }
    if(rb_derecho(nodo) != arbol->nil) {
        rb_destruir_nodo(arbol, rb_derecho(nodo));
    }
    free(nodo);
}

rb_nodo* rb_buscar_nodo(rb_arbol* arbol, int valor, rb_nodo* nodo) {
    if(nodo == arbol->nil || rb_datos(nodo) == valor) {
        return nodo;
    }
    if(valor > rb_datos(nodo)) {
        return rb_buscar_nodo(arbol, valor, rb_derecho(nodo));
    } else {
        return rb_buscar_nodo(arbol, valor, rb_izquierdo(nodo));
    }
}

rb_nodo* rb_buscar(rb_arbol* arbol, int valor) {
    return rb_buscar_nodo(arbol, valor, arbol->raiz);
}



void rb_rotar_derecha(rb_arbol* arbol, rb_nodo* x) {
    rb_nodo* y = rb_izquierdo(x);
    rb_set_izquierdo(arbol, x, rb_derecho(y));
    rb_set_padre(y, rb_padre(x));
    if(rb_arbol_raiz(arbol) == x) {
        rb_arbol_set_raiz(arbol, y);
    } else {
        if(x == rb_derecho(rb_padre(x))) {
            rb_set_derecho(arbol, rb_padre(x), y);
        } else {
            rb_set_izquierdo(arbol, rb_padre(x), y);
        }  
    }
    rb_set_derecho(arbol, y, x);
}

void rb_rotar_izquierda(rb_arbol* arbol, rb_nodo* x) {
    rb_nodo* y = rb_derecho(x);
    rb_set_derecho(arbol, x, rb_izquierdo(y));
    rb_set_padre(y, rb_padre(x));
    if(rb_arbol_raiz(arbol) == x) {
        rb_arbol_set_raiz(arbol, y);
    } else {
        if(x == rb_izquierdo(rb_padre(x))) {
            rb_set_izquierdo(arbol, rb_padre(x), y);
        } else {
            rb_set_derecho(arbol, rb_padre(x), y);
        }
    }
    rb_set_izquierdo(arbol, y, x);
}

rb_nodo* rb_maximo(rb_arbol* arbol) {
    return rb_maximo_nodo(arbol, arbol->raiz);
}
rb_nodo* rb_maximo_nodo(rb_arbol* arbol, rb_nodo* nodo) {
    if(rb_derecho(nodo) == arbol->nil) {
        return nodo;
    }
    return rb_maximo_nodo(arbol, rb_derecho(nodo));
}

rb_nodo* rb_minimo(rb_arbol* arbol) {
    return rb_minimo_nodo(arbol, arbol->raiz);
}
rb_nodo* rb_minimo_nodo(rb_arbol* arbol, rb_nodo* nodo) {
    if(rb_izquierdo(nodo) == arbol->nil) {
        return nodo;
    }
    return rb_minimo_nodo(arbol, rb_izquierdo(nodo));
}

rb_nodo* rb_sucesor(rb_arbol* arbol, rb_nodo* nodo) {
    if(nodo == arbol->nil || rb_derecho(nodo) == arbol->nil) {
        return nodo;
    }
    return rb_minimo_nodo(arbol, rb_derecho(nodo));
}

rb_nodo* rb_predecesor(rb_arbol* arbol, rb_nodo* nodo) {
    if(nodo == arbol->nil || rb_izquierdo(nodo) == arbol->nil) {
        return nodo;
    }
    return rb_maximo_nodo(arbol, rb_izquierdo(nodo));
}



void rb_arreglar_insercion(rb_arbol* arbol, rb_nodo* z) {
    rb_nodo* y;/* nodo tio */
    while(rb_color(rb_padre(z)) == ROJO) {
        if(rb_padre(z) == rb_izquierdo(rb_padre(rb_padre(z)))) {
            y = rb_derecho(rb_padre(rb_padre(z)));
            /*
            Caso 1:
            Violamos la propiedad de que un rojo no tiene un hijo rojo.
            Vamos a recolorear y mover la violación hacia arriba.
            En la proxima iteración me fijaré que rompí. 
     
            */
            if(rb_color(y) == ROJO) {
                rb_set_color(rb_padre(z), NEGRO);
                rb_set_color(y, NEGRO);
                rb_set_color(rb_padre(rb_padre(z)), ROJO);
                /* 
                Me muevo dos lugares para arriba, que es donde
                puedo haber dejado inconsistencias, porque
                pinté a mi abuelo de rojo sin mirar a su padre.
                */
                z = rb_padre(rb_padre(z));
            } else {
                /*
                Caso 2:
                Tengo una violación, pero quizás estoy en zig-zag con mi abuelo.
                Hago una rotación para pasarlo todo a una linea, y 
                la violación (2 rojos) ahora es entre mi abuelo y mi padre.
                */
                if (z == rb_derecho(rb_padre(z))) {
                    /*
                    Mi padre es el hijo izquierdo, pero mi tío es negro.
                    Roto para que la violación vaya para mi hijo izquierdo.
                    */
                    z = rb_padre(z);
                    rb_rotar_izquierda(arbol, z);
                }
                /*
                Caso 3:
                Tengo una violación de 2 rojos entre mi abuelo y mi padre.
                Cambio los colores, y roto.
                */
                rb_set_color(rb_padre(z), NEGRO);
                rb_set_color(rb_padre(rb_padre(z)), ROJO);
                y = rb_padre(rb_padre(z));
                rb_rotar_derecha(arbol, y);                   
            }
        } else {
            /* Casos simétricos.*/
            
            y = rb_izquierdo(rb_padre(rb_padre(z)));
            if(rb_color(y) == ROJO) {
                rb_set_color(rb_padre(z), NEGRO);
                rb_set_color(y, NEGRO);
                rb_set_color(rb_padre(rb_padre(z)), ROJO);
                z = rb_padre(rb_padre(z));
            } else {
                if (z == rb_izquierdo(rb_padre(z))) {
                    z = rb_padre(z);
                    rb_rotar_derecha(arbol, z);
                }
                 
                rb_set_color(rb_padre(z), NEGRO);
                rb_set_color(rb_padre(rb_padre(z)), ROJO);
                y = rb_padre(rb_padre(z));
                
                rb_rotar_izquierda(arbol, y);

            }
        }
    }
    rb_set_color(arbol->raiz, NEGRO);
}

int rb_insertar_nodo(rb_arbol* arbol, rb_nodo* nodo, int valor) {
    int datos;
    if(rb_arbol_raiz(arbol) == arbol->nil) {
        rb_arbol_set_raiz(arbol, rb_nuevo(arbol, arbol->nil, arbol->nil, NEGRO, valor));
        return 1;
    }
    datos = rb_datos(nodo);
    if(datos == valor) {
        return 0;
    }
    if(valor > datos) {
        if(rb_derecho(nodo) != arbol->nil) {
            return rb_insertar_nodo(arbol, rb_derecho(nodo), valor);
        } else {
            rb_nodo* nuevo = rb_nuevo(arbol, arbol->nil, arbol->nil, ROJO, valor);
            rb_set_derecho(arbol, nodo, nuevo);
            rb_arreglar_insercion(arbol, nuevo);
            return 1; 
	}
    } else {
        if(rb_izquierdo(nodo) != arbol->nil) {
            return rb_insertar_nodo(arbol, rb_izquierdo(nodo), valor);
        } else {
            rb_nodo* nuevo = rb_nuevo(arbol, arbol->nil, arbol->nil, ROJO, valor);
            rb_set_izquierdo(arbol, nodo, nuevo);
            rb_arreglar_insercion(arbol, nuevo);
            return 1;
        }
    }
}

int rb_insertar(rb_arbol* arbol, int valor) {
    return rb_insertar_nodo(arbol, arbol->raiz, valor);
}

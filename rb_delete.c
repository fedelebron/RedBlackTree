#include <assert.h>
#include "rbtree.h"

void rb_eliminar(rb_arbol* arbol, int valor) {
    if(rb_datos(rb_arbol_raiz(arbol)) == valor && rb_es_hoja(arbol, rb_arbol_raiz(arbol))) {
        free(arbol->raiz);
        arbol->raiz = arbol->nil;
        return;
    }
    rb_eliminar_nodo(arbol, rb_buscar(arbol, valor));
}


void rb_eliminar_nodo(rb_arbol* arbol, rb_nodo* nodo) {
    /*
        Primero eliminamos el nodo de la misma manera
        que en un arbol binario de búsqueda normal.
        
        Encontramos el nodo a borrar. Si tiene hijos,
        lo swapeamos con su sucesor o predecesor 
        inmediato. Luego, borramos el nodo, que ahora
        va a tener:
            a)  0 hijos, y ser rojo
            b)  0 hijos, y ser negro
            c)  1 hijo, y ser negro, y su hijo
                una hoja roja
                
        Si es rojo, lo sacamos y listo.
        Si es negro con un hijo rojo, y swapeamos los 
        colores de su padre y su hijo.
        
        Si tiene 0 hijos y es negro, la pasamos como 
        el orto, porque al sacar el elemento del arbol 
        estamos cambiando la altura negra de un subarbol.
    */  
    
    
    /*
        Lo que me pasaron acá es un puntero al nodo
        en cuestión - no necesito loopear para buscarlo.
    */
    
    unsigned int hijos = rb_contar_hijos(arbol, nodo);
    rb_nodo* pre;
    rb_nodo* punta;
    rb_nodo* hermano;
    int orig;
    
    orig = rb_datos(nodo);
    if(hijos == 2) {
        pre = rb_predecesor(arbol, nodo);
        rb_swap(arbol, nodo, pre);
        nodo = pre;
    }
    
    if(rb_color(nodo) == ROJO) {
        /*assert(rb_es_hoja(arbol, nodo));*/
        if(!(rb_es_hoja(arbol, nodo))) {
            printf("No lo cambie por su pre. El es: %d. Su predecesor es: %d.\n", orig, rb_datos(nodo));
            rb_dump_arbol(arbol);
            exit(1);
        }
        /*
            Caso a:
            Sin hijos, rojo.
            Sacamos al toque.
        */
        
        rb_eliminar_hijo(arbol, rb_padre(nodo), nodo);
        return;
    }
    
    if( rb_color(nodo) == NEGRO &&
        rb_contar_hijos(arbol, nodo) == 1) {
        
        /*assert(rb_derecho(nodo) == arbol->nil);
        assert(rb_color(rb_izquierdo(nodo)) == ROJO);
        */
        /*
            Caso b:
            Un hijo a la izquierda, rojo.
            Yo negro.
            Cambiamos los valores, y borramos al rojo.
        */
        if(rb_derecho(nodo) == arbol->nil) {            
            rb_set_datos(nodo, rb_datos(rb_izquierdo(nodo)));
            rb_eliminar_hijo(arbol, nodo, rb_izquierdo(nodo));
        } else {            
            rb_set_datos(nodo, rb_datos(rb_derecho(nodo)));
            rb_eliminar_hijo(arbol, nodo, rb_derecho(nodo));
        }
        return;
    }
    
    

    
    if( rb_color(nodo) == NEGRO ) {
        assert(rb_es_hoja(arbol, nodo));
        
        /*
            Caso c:
            Es una hoja negra.
            
            Existe un sub-arbol del arbol tal que
            cambié la altura negra del sub-arbol.
            
            Llamemos punta al nodo padre de este 
            sub-arbol. En este momento, punta es el
            padre del nodo que sacamos. A medida
            que haga correcciones, este sub-arbol
            va a ir creciendo.
        
        */
        punta = rb_padre(nodo);
        hermano = rb_hermano(nodo);
        rb_eliminar_hijo(arbol, punta, nodo);

        /*
            Asumo que soy el hijo derecho de mi padre.
            Los otros casos son simétricos.
        */
        
        rb_arreglar_eliminacion(arbol, punta, hermano);
    }        
}

void rb_arreglar_eliminacion(rb_arbol* arbol, rb_nodo* punta, rb_nodo* hermano) {
    rb_nodo* sobrino;
    rb_nodo* sobrino_nieto;
    

    if(rb_izquierdo(punta) == hermano) {
        if(rb_color(punta) == ROJO) {
            /*
                Caso c.1.1:
                Mi padre es rojo. Entonces, mi 
                hermano es negro.
            */ 
            
            /* Mi hermano tiene un hijo rojo? */
            if((rb_izquierdo(hermano) != arbol->nil && rb_color(rb_izquierdo(hermano)) == ROJO)) {
            
                /*
                El arbol tiene esta pinta:
                                P
                              /  \
                            H     z
                          /  \
                        C     y
                      /  \
                    w    x
                P = padre, H = hermano, C es rojo.
                z es donde yo estaba, que pasa de tener
                altura negra h a h-1.
                w, x e y todos tienen altura negra h-1.
                    
                */
                rb_rotar_derecha(arbol, punta);
                
                /*
                El arbol ahora tiene esta pinta:
                            H
                          /  \ 
                        C     P
                      /  \   / \
                    w     x y   z
                
                Es posible que y fuera rojo, entonces tengo
                que cambiarle el color a P. Pero hacer eso
                cambia la altura negra de z e y. Entonces
                pinto también a C de negro. Pero ahora w, x,
                y, z tienen altura h+1, y necesito que sea h.
                Entonces, pinto a H de rojo (no violo nada
                porque si el padre de H es rojo, entonces el
                padre de P antes era rojo, absurdo porque P
                era rojo).
                */
                rb_set_color(punta, NEGRO);
                rb_set_color(hermano, ROJO);
                rb_set_color(rb_izquierdo(hermano), NEGRO);
                
                /*
                    Ahora w, x, y, z tienen altura h. Terminé.
                */
            } else if((rb_derecho(hermano) != arbol->nil && rb_color(rb_derecho(hermano)) == ROJO)) {
                /* El hijo derecho de mi hermano es rojo.
                Entonces, el arbol tiene esta pinta:
                               P
                             /  \
                            H    z
                          /  \
                         w    C
                             | \
                            x   y
                Donde P es punta, H es hermano, C es rojo.
                Yo solía ser z.
                
                Quiero llevarlo a algo de la forma...   
                
                                C
                              /  \ 
                            H     P
                          /  \   / \
                        w     x y   z                 
                */
                
                               
                rb_rotar_izquierda(arbol, hermano); 
                
                /* El arbol ahora se ve así:
                                    P
                                  /  \
                                C     z
                              /  \
                            H     y
                          /  \
                        w    x 
                  
                  El izquierdo de la punta es lo que antes era
                  el derecho del hermano.
                */
                rb_rotar_derecha(arbol, punta);
                
                /* Ahora se vé así:
                                C
                              /  \ 
                            H     P
                          /  \   / \
                        w     x y   z                     
                
                
                Que es alo que queríamos llegar. Pero P y H
                tenían colores distintos. Entonces recoloreo
                para que todos los subarboles tengan altura h.
                */
                
                rb_set_color(punta, NEGRO);
                rb_set_color(hermano, NEGRO);
                rb_set_color(rb_padre(hermano), ROJO);
            } else {
                /* Caso c.1.2:
                Mi hermano no tiene un hijo rojo, y mi padre es rojo.
                Entonces, es fácil. El problema tiene esta pinta:
                            P
                          /  \
                         H    z
                       /  \    
                     x     y
                     
                 Donde x e y son negros, H es negro, P es rojo.
                 Pero entonces, simplemente recoloreamos P y H.
                 z gana una altura, x e y permanecen iguales.
             
                */
             
                rb_set_color(punta, NEGRO);
                rb_set_color(hermano, ROJO);
            }
        } else {
        
            /*  Caso c.2:
                Mi padre es negro.
            */
            
            /* Mi hermano es rojo? */
            if(rb_color(hermano) == ROJO) {
                /*  Caso c.2.1:
                    Mi hermano es rojo.
                */
                
                /* Mi sobrino es negro. */
                sobrino = rb_derecho(hermano);
                /* Tengo un sobrino-nieto (hijo de mi sobrino) rojo? */ 

                if(rb_izquierdo(sobrino) != arbol->nil && rb_color(rb_izquierdo(sobrino)) == ROJO) {
                    /*  Caso c.2.1.1: Tengo un sobrino-nieto rojo. 
                        El arbol tiene esta pinta:
                                    P
                                  /  \
                                H     z
                              /  \
                            v     S
                                 / \
                               SN   y
                              /  \  
                             w    x
                        z es donde yo solía estar.
                        Colores:
                            P  : Negro
                            H  : Rojo
                            S  : Negro
                            SN : Rojo
                            
                        Alturas negras de sub-arboles: 
                        
                            v : h
                            w : h-1
                            x : h-1
                            y : h-1
                            z : h -> h-1
                        
                        Para arreglar esto, hago dos rotaciones:
                    */
                    sobrino_nieto = rb_izquierdo(sobrino);
                    
                    rb_rotar_izquierda(arbol, hermano);
                    
                    /*
                        Ahora el arbol tiene esta pinta:
                                    P
                                  /  \
                                S     z
                              /  \
                            H     y
                           / \
                          v  SN
                            /  \
                           w    x
                           
                        Una rotación más y estamos...
                    
                    */
                    
                    rb_rotar_derecha(arbol, punta);
                    
                    /*
                        Ahora se vé así:
                                  S
                                /  \
                              H     P
                            / \    / \
                          v    \  y   z
                               SN
                              /  \
                             w    x
                        Y yá está balanceado.
                        Pero w y x tienen muy poca altura.
                        Entonces convertimos SN a negro.
                    */
                    
                    rb_set_color(sobrino_nieto, NEGRO);
                
                } else if(rb_derecho(sobrino) != arbol->nil && rb_color(rb_derecho(sobrino)) == ROJO) {
                    /* Caso c.2.1.1: Tengo un sobrino-nieto rojo. */
            
                   sobrino_nieto = rb_derecho(sobrino);

                    /* Parecido, solo que tengo que mover mas cosas: */
                    
                    /*  El arbol ahora tiene esta pinta:
                    
                                P
                              /  \
                            H     z
                          /  \
                         v    S
                            /  \
                           w   SN
                              /  \
                            x     y
                    
                    
                    Puedo rotar un par de veces...
                    */
                    
                    rb_rotar_izquierda(arbol, sobrino);
                    /*
                        Ahora tenemos esto:
                        
                                P
                              /  \
                            H     z
                           / \
                          v  SN
                             / \
                            S   y
                           / \ 
                          w  x
        
                        Igual que en el caso anterior, rotamos y recoloreamos.
                    */
                    rb_rotar_derecha(arbol, punta);
                    rb_rotar_derecha(arbol, punta);
                    rb_set_color(hermano, NEGRO);
                    
                } else {
                    /* No tengo un sobrino-nieto rojo. */
                    
                    /* El arbol se parece a esto:
                                P
                              /  \
                            H     z
                          /  \
                         w    S
                             / \
                            x   y
                            
                        Donde x e y tienen puntas negras.
                        
                        Podemos rotar y colorear 1 vez, y funca.
                    */
                    
                    rb_rotar_derecha(arbol, punta);
                    rb_set_color(hermano, NEGRO);
                    rb_set_color(sobrino, ROJO);  /* está bien, porque sus dos hijos eran negros */
                }
                
                
            } else {
                /* Mi hermano no es rojo. */
                
                /* Tengo un sobrino rojo? */
                if(rb_derecho(hermano) != arbol->nil && rb_color(rb_derecho(hermano)) == ROJO) {
                    sobrino = rb_derecho(hermano);
                    rb_rotar_izquierda(arbol, hermano);
                    rb_rotar_derecha(arbol, punta);
                    rb_set_color(sobrino, NEGRO);
                } else if(rb_izquierdo(hermano) != arbol->nil && rb_color(rb_izquierdo(hermano)) == ROJO) {
                    sobrino = rb_izquierdo(hermano);
                    rb_rotar_derecha(arbol, punta);
                    rb_set_color(sobrino, NEGRO);
                } else {
                    /*  Solo tengo que recolorear a mi hermano
                        de rojo. Pero este sub-arbol acaba de cagarla.
                        Entonces, vuelvo a empezar este procedimiento.
                    */

                    rb_set_color(hermano, ROJO);
                    if(punta == rb_arbol_raiz(arbol)) {
                        rb_set_color(punta, NEGRO);
                        return;
                    }
                    rb_arreglar_eliminacion(arbol, rb_padre(punta), rb_hermano(punta));
                }
            
            }
        
        }
    } else {
        /* Casos simétricos */
        if(rb_color(punta) == ROJO) {
            if((rb_derecho(hermano) != arbol->nil && rb_color(rb_derecho(hermano)) == ROJO)) {
                rb_rotar_izquierda(arbol, punta);
                rb_set_color(punta, NEGRO);
                rb_set_color(hermano, ROJO);
                rb_set_color(rb_derecho(hermano), NEGRO);
            } else if((rb_izquierdo(hermano) != arbol->nil && rb_color(rb_izquierdo(hermano)) == ROJO)) {
                rb_rotar_derecha(arbol, hermano);
                rb_rotar_izquierda(arbol, punta);
                rb_set_color(punta, NEGRO);
                rb_set_color(hermano, NEGRO);
                rb_set_color(rb_padre(hermano), ROJO);
            } else {                
                rb_set_color(punta, NEGRO);
                rb_set_color(hermano, ROJO);
            }
        } else {  
            if(rb_color(hermano) == ROJO) {
                sobrino = rb_izquierdo(hermano);
                if(rb_derecho(sobrino) != arbol->nil && rb_color(rb_derecho(sobrino)) == ROJO) {
                    sobrino_nieto = rb_derecho(sobrino);
                    rb_rotar_derecha(arbol, hermano);
                    rb_rotar_izquierda(arbol, punta);
                    rb_set_color(sobrino_nieto, NEGRO);                    
                } else if(rb_izquierdo(sobrino) != arbol->nil && rb_color(rb_izquierdo(sobrino)) == ROJO) {
                    sobrino_nieto = rb_izquierdo(sobrino);
                    rb_rotar_derecha(arbol, sobrino);
                    rb_rotar_izquierda(arbol, punta);
                    rb_rotar_izquierda(arbol, punta);
                    rb_set_color(hermano, NEGRO);
                } else {
                    rb_rotar_izquierda(arbol, punta);
                    rb_set_color(hermano, NEGRO);
                    rb_set_color(sobrino, ROJO);
                }              
            } else {
                if(rb_izquierdo(hermano) != arbol->nil && rb_color(rb_izquierdo(hermano)) == ROJO) {
                    sobrino = rb_izquierdo(hermano);
                    rb_rotar_derecha(arbol, hermano);
                    rb_rotar_izquierda(arbol, punta);
                    rb_set_color(sobrino, NEGRO);
                } else if(rb_derecho(hermano) != arbol->nil && rb_color(rb_derecho(hermano)) == ROJO) {
                    sobrino = rb_derecho(hermano);
                    rb_rotar_izquierda(arbol, punta);
                    rb_set_color(sobrino, NEGRO);
                } else {
                    rb_set_color(hermano, ROJO);
                    if(punta == rb_arbol_raiz(arbol)) {
                        rb_set_color(punta, NEGRO);
                        return;
                    }
                    rb_arreglar_eliminacion(arbol, rb_padre(punta), rb_hermano(punta));
                }
            }
        }
    }
}
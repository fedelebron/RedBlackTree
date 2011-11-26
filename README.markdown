This is a red-black tree implementation in C.

It was meant as an exercise in beginning to get comfortable with C. It is partially documented (the deletion part excruciatingly so, more for me to understand what was happening than for the reader to), in Spanish, the same language the functions are written in.

To compile, type "make". You will need GNU make or a compatible make utility. You may edit the Makefile to choose alternative compilers or compiler flags.

The main functions to manipulate a tree are:

* rb_arbol* rb_nuevo_arbol(int n) -- Create a new tree with a given integer n as root.
* void rb_destruir_arbol(rb_arbol* t) -- Delete a tree t and free up its memory. This recursively deletes every node in the tree.
* void rb_dump_arbol(rb_arbol* t) -- Dump a tree t to graphviz .dot format to stdout.
* int rb_insertar(rb_arbol* t, int n) -- Insert an integer n into the tree t. Returns 1 if it succeeds, 0 otherwise.
* void rb_eliminar(rb_arbol* t, int n) -- Delete a node from the tree t, given its value n. The node must already be in the tree.
* rb_nodo* rb_buscar(rb_arbol* t, int n) -- Find a node in the tree t, given its value n. If the node does not exist, the function returns t->nil.
* rb_nodo* rb_maximo(rb_arbol* t) -- Find the largest (numerically) node in the tree t.
* rb_nodo* rb_minimo(rb_arbol* t) -- Find the smallest (numerically) node in the tree t.
* rb_nodo* rb_sucesor(rb_arbol* t, rb_nodo* n) -- Find the successor of the node n in the tree t.
* rb_nodo* rb_predecesor(rb_arbol* t, rb_nodo* n) -- Find the predecessor of the node n in the tree t.

Internally, you may traverse the tree with the following functions:

* rb_nodo* rb_arbol_raiz(rb_arbol*) -- Get a tree's root node.
* rb_nodo* rb_hermano(rb_nodo*) -- Get a node's sibling.
* rb_nodo* rb_padre(rb_nodo*) -- Get a node's parent.
* rb_color_nodo rb_color(rb_nodo*) -- Get a node's color.
* rb_nodo* rb_derecho(rb_nodo* n) and rb_nodo* rb_set_derecho(rb_arbol\*, rb_nodo\* n, rb_nodo\* m) -- Get and set node n's right child.
* rb_nodo* rb_izquierdo(rb_nodo* n) and rb_nodo* rb_set_izquierdo(rb_arbol\*, rb_nodo\* n, rb_nodo\* m) -- Get and set node n's left child.   
* int rb_datos(rb_nodo* n) and int rb_set_datos(rb_nodo* n, int) -- Get and set node n's value. In both cases its value is returned.

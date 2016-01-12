#ifndef LISTAS_H_INCLUDED
#define LISTAS_H_INCLUDED

// Una lista individual
typedef struct {
	int tamano;
	int* num;
} lista;


// Imprime una lista
void mostrarlista(lista* lista);


#endif
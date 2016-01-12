#ifndef LISTAS_H_INCLUDED
#define LISTAS_H_INCLUDED

// Una lista individual
typedef struct {
	int tamano;
	int* num;
} lista;


// Conjunto de listas
typedef struct {

	int cuantas_listas;
	lista* li;

} listas;


// Imprime una lista
void mostrarlista(listas* conjunto_listas, int indice);

// Crea las listas
void inicializar_listas(listas* listas_struct, char* nombre_archivo);

#endif
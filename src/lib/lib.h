#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include "lista.h"

// Toma como argumento un arreglo de listas, y las ordena
// segun el tamano de cada una (de menor a mayor)
void quicksort_arreglo_listas(lista* listas, int tamano);

// Toma una lista, y ordena sus elementos de menor a mayor
void quicksort_lista(lista* lista);

// Toma una lista, y ordena sus elementos de menor a mayor
// En este caso se le puede pasar el limite inferior e inferior
void quicksort_lista_limites(lista* lista, int lo, int hi);

// Booleano: Usando busqueda binaria (iterativa) determina si un valor esta en la lista
int existe_elemento_en_busquedabinaria(int valor, lista* lista);

#endif
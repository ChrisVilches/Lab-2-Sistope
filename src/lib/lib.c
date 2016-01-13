#include "lib.h"
#include "lista.h"
#include <stdio.h>

int tres_primeros_lugares(int* arreglo_a_rellenar, int cantidad_total_equipos, double* tiempos){

    int arreglo_ids[cantidad_total_equipos];
    double tiempos_copia[cantidad_total_equipos]; // Crear copia de los tiempos
    int i;
    int j;

    double tiempo_temp;
    int id_temp;

    // Rellenar con valores por defecto
    arreglo_a_rellenar[0] = -1;
    arreglo_a_rellenar[1] = -1;
    arreglo_a_rellenar[2] = -1;

    // Enumerar las IDs
    for(i=0; i<cantidad_total_equipos; i++){
        arreglo_ids[i] = i;
    }

    // Crear copia de los tiempos
    for(i=0; i<cantidad_total_equipos; i++){
        tiempos_copia[i] = tiempos[i];
    }


    // Algoritmo de ordenamiento por insercion
    for(i=0; i<cantidad_total_equipos; i++){
        j=i;
        while(j>0 && tiempos_copia[j-1]>tiempos_copia[j]){
            tiempo_temp = tiempos_copia[j];
            id_temp = arreglo_ids[j];

            arreglo_ids[j] = arreglo_ids[j-1];
            tiempos_copia[j] = tiempos_copia[j-1];

            arreglo_ids[j-1] = id_temp;
            tiempos_copia[j-1] = tiempo_temp;
            j--;
        }
    }
    
    for(i=0; i<cantidad_total_equipos && i<3; i++){
        arreglo_a_rellenar[i] = arreglo_ids[i];
    }    

}


int existe_elemento_en_busquedabinaria(int valor, lista* lista, int inf, int sup){

    int l = inf;
    int r = sup;

    while (l <= r)
    {
        int m = l + (r-l)/2;
         
        if (lista->num[m] == valor) 
            return 1;  
         
        if (lista->num[m] < valor) 
            l = m + 1;
         
        else 
            r = m - 1;
    }

    return 0;
}


void intercambiar_listas(lista* listas, int i, int j){

    lista temp = listas[i];
    listas[i] = listas[j];
    listas[j] = temp;    

}


int quicksort_particion(lista* listas, int lo, int hi){
	int pivote = listas[hi].tamano;
	int i = lo;
    int j;

    for(j=lo; j<hi; j++){
    	if(listas[j].tamano <= pivote){
            intercambiar_listas(listas, i, j);
            i++;
    	}
    }
    intercambiar_listas(listas, i, hi);
	return i;
}

void quicksort_arreglo_listas_limites(lista* listas, int lo, int hi){
    int p;
    if(lo < hi){
        p = quicksort_particion(listas, lo, hi);
        quicksort_arreglo_listas_limites(listas, lo, p-1);
        quicksort_arreglo_listas_limites(listas, p+1, hi);
    }
}

// Sirve para ordenar un arreglo de listas por tamano de menor a mayor
void quicksort_arreglo_listas(lista* listas, int tamano){
    quicksort_arreglo_listas_limites(listas, 0, tamano-1);
}


int quicksort_lista_particiones(lista* lista, int lo, int hi){
    int pivote = lista->num[hi];
    int i = lo;
    int j;
    int temp;

    for(j=lo; j<hi; j++){
        if(lista->num[j] <= pivote){
            temp = lista->num[i];
            lista->num[i] = lista->num[j];
            lista->num[j] = temp;
            i++;
        }
    }
    temp = lista->num[i];
    lista->num[i] = lista->num[hi];
    lista->num[hi] = temp;
    return i;

}

// Toma una lista, y ordena sus elementos de menor a mayor
// En este caso se le puede pasar el limite inferior e inferior
void quicksort_lista_limites(lista* lista, int lo, int hi){
    int p;
    if(lo < hi){
        p = quicksort_lista_particiones(lista, lo, hi);
        quicksort_lista_limites(lista, lo, p-1);
        quicksort_lista_limites(lista, p+1, hi);
    }
}

// Toma una lista, y ordena sus elementos de menor a mayor
void quicksort_lista(lista* lista){
    quicksort_lista_limites(lista, 0, lista->tamano-1);
}
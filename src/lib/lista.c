#include <stdio.h>
#include "lista.h"

// Imprime una lista
void mostrarlista(lista* lista){
	int i;
	for(i=0; i<lista->tamano; i++){
		printf("%d ", lista->num[i]);
	}
	printf("\n");
}
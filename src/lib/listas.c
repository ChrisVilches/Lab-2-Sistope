#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "listas.h"


// Metodo que muestra una lista del conjunto de listas

void mostrarlista(listas* conjunto_listas, int indice){
	int i;
	for(i=0; i<conjunto_listas->li[indice].tamano; i++){
		printf("%d ", conjunto_listas->li[indice].num[i]);
	}
	printf("\n");
}


void inicializar_listas(listas* listas_struct, char* nombre_archivo){

	FILE* fp;
	char line[4096];
	char line2[4096];
	int longitud_lista;
	char* token;
	int cantidad_listas = 0;
	int i = 0;
	int j;

	fp = fopen(nombre_archivo, "r");

	// Validar el archivo
	if(fp == NULL){
		printf("Archivo no encontrado: %s\n", nombre_archivo);
		abort();
	}

	// Contar la cantidad de lineas que tiene el archivo
	while (fgets(line, sizeof(line), fp) != NULL){
		cantidad_listas++;
	}
	
	// Crear el conjunto de listas
	listas_struct->li = (lista*) malloc(sizeof(lista) * cantidad_listas);
	listas_struct->cuantas_listas = cantidad_listas;

	// Volver al inicio del archivo
	rewind(fp);

	// Crear las listas
	while (fgets(line, sizeof(line), fp) != NULL){

		j=1;

		// Copiar la linea
		strcpy(line2, line);

		// Contar cuantos numeros tiene la lista
		longitud_lista = 0;
		token = strtok(line, " ");
		while(token != NULL && token[0] >= '0' && token[0] <= '9') {			
			token = strtok(NULL, " ");
			longitud_lista++;
		}

		// Crear una nueva lista del tamano leido
		lista nueva;
		nueva.tamano = longitud_lista;
		nueva.num = (int*) malloc(sizeof(int) * longitud_lista);

		// Cortar nuevamente la string, para poder agregar los numeros
		token = strtok(line2, " ");
		nueva.num[0] = atoi(token);

		while(token != NULL) {	
			token = strtok(NULL, " ");	
			if(token != NULL && token[0] >= '0' && token[0] <= '9'){
				nueva.num[j] = atoi(token);
			} else {
				break;
			}	

			j++;
		}

		// Guardar esta lista en la posicion adecuada
		// en el conjunto de listas
		listas_struct->li[i] = nueva;
		i++;
	}
	fclose(fp);	
}



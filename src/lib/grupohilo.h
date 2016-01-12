#ifndef GRUPOHILO_H_INCLUDED
#define GRUPOHILO_H_INCLUDED
#include "lista.h"
#include "monitor.h"
#include <pthread.h>

typedef struct {

	// Numero de hilos por grupo
	int num_threads;

	// Cada grupo lee el archivo de texto, y mantiene su propia copia de las listas
	lista* conjunto_listas;

	// Cantidad de listas
	int cuantas_listas;

	// El monitor que usara el equipo
	monitor monitor;

	// Arreglo de hilos
	pthread_t* hilos;

	// Tiempos de cada hebra
	double* tiempo_hebra;

} grupohilo;

// Comienza a intersectar listas
void intersectar_listas(grupohilo* grupohilo, int* mejor_hebra, double* promedio_tiempos);

// Constructor
void inicializar_grupohilo(grupohilo* grupohilo, char* nombre_archivo, int threads_por_equipo);

// Crea las listas
void leer_listas(grupohilo* grupohilo, char* nombre_archivo);

#endif 
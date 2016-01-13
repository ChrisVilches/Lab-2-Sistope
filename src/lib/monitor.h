#ifndef MONITOR_H_INCLUDED
#define MONITOR_H_INCLUDED

#include <pthread.h>
#include "lista.h"

typedef struct {

	int cuantos_hilos;
	int cuantas_listas;

	// Debe comenzar con la lista 0, la lista 0 es la mas corta (se hizo Quicksort previamente)
	int lista_actual;


	// Provee exclusion mutua para acceder la lista S'
	pthread_mutex_t semaforo_sprima;

	// Provee exclusion mutua para incrementar el valor de cuantas hebras han terminado de procesar la sublista k
	pthread_mutex_t semaforo_sublistak;

	// Posicion actual en la lista S prima, sirve para ir agregando elementos al final de la lista (para saber donde va)
	int pos_sprima;

	// Tamano maximo lista S'. Se crea del mismo tamano que la lista S, pero varia con realloc
	int tamano_sprima;

	// Puntero a la lista S'. Por simplicidad, se deja como arreglo de enteros.
	int* s_prima;

	// Cuantos han terminado
	int cuantos_han_terminado;

	// Variable de condicion
	pthread_cond_t todos_terminaron;

	// Interseccion es no vacia?
	int interseccion_no_vacia;


} monitor;


// Comienza el monitor con sus valores iniciales
void inicializar_monitor(monitor* monitor, int cuantos_hilos, int cuantas_listas);

// Acceso exclusivo a la lista S' para agregar un elemento en ella
void agregar_elemento_sprima(monitor* monitor, int numero);

// Booleano: Retorna verdadero si la quedan listas que examinar
int quedan_listas(monitor* monitor);

// El monitor crea una lista S'. Puede ser ejecutada por varios hilos, pero solo uno funciona, y los demas no hacen nada
void monitor_crear_lista_s_prima(monitor* monitor, int tamano);

// Se le avisa al monitor que una hebra ha terminado de procesar una sublista K
void monitor_termine_de_procesar_una_sublista_k(monitor* monitor, lista* S, int id_hilo);

// Booleano, entrega verdadero si la interseccion no es vacia
int comprobar_interseccion_no_vacia(monitor* monitor);


#endif

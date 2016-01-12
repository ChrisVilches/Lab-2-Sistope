#ifndef MONITOR_H_INCLUDED
#define MONITOR_H_INCLUDED

#include <pthread.h>

typedef struct {

	int cuantos_hilos;
	int cuantas_listas;

	// Debe comenzar con la lista 1, la lista 0 es la mas corta (se hizo Quicksort previamente)
	int lista_actual;

	// Provee exclusion mutua para acceder la lista S
	pthread_mutex_t semaforo_s;

	// Provee exclusion mutua para acceder la lista S'
	pthread_mutex_t semaforo_sprima;

	// Posicion actual en la lista S prima, sirve para ir agregando elementos al final de la lista (para saber donde va)
	int pos_sprima;

	// Tamano lista S'. Se puede determinar ya que es igual a la lista mas corta del grupo.	
	// Recordar que se deben generar (malloc) varias listas S', ya que al final del algoritmo se hace S=S'
	// Por lo que no se puede reutilizar despues.
	// Para futuras listas, este tamano puede variar (si la interseccion entrega un numero menor de elementos)
	// Por eso, este valor no se inicializa al comienzo, si no que se va variando con el tiempo.
	int tamano_sprima;

	// Puntero a la lista S'
	int* s_prima;

} monitor;


void inicializar_monitor(monitor* monitor, int cuantos_hilos, int cuantas_listas);

void agregar_elemento_sprima(int numero);


#endif

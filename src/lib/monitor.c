#include <stdlib.h>
#include <stdio.h>
#include "monitor.h"

void inicializar_monitor(monitor* monitor, int cuantos_hilos, int cuantas_listas){


	monitor->cuantos_hilos = cuantos_hilos;
	monitor->cuantas_listas = cuantas_listas;
	monitor->lista_actual = 1;
	monitor->pos_sprima = 0;
	monitor->cuantos_han_terminado = 0;
	monitor->s_prima = NULL;
	monitor->interseccion_no_vacia = 1;
	monitor->tamano_sprima = 0;

	pthread_mutex_init(&monitor->semaforo_sublistak, NULL);
	pthread_mutex_init(&monitor->semaforo_sprima, NULL);

	pthread_cond_init(&monitor->todos_terminaron, NULL);

	pthread_mutex_unlock(&monitor->semaforo_sublistak);
	pthread_mutex_unlock(&monitor->semaforo_sprima);


}


void agregar_elemento_sprima(monitor* monitor, int numero){

	pthread_mutex_lock(&monitor->semaforo_sprima);

	// Si pos sprima es igual al tamano (posicion s prima significa que hay pos_sprima elementos ya en la lista)
	// hay que hacer un realloc
	if(monitor->pos_sprima == monitor->tamano_sprima){

		// Utilizar el doble del tamano anterior
		monitor->tamano_sprima *= 2;

		monitor->s_prima = (int*) realloc(monitor->s_prima, sizeof(int) * monitor->tamano_sprima);

		if(monitor->s_prima == NULL){
			printf("Hubo un error al hacer realloc.\n");
			abort();
		}
	}

	// Agregar elemento y cambiar el puntero pos_sprima

	monitor->s_prima[monitor->pos_sprima] = numero;

	monitor->pos_sprima++;

	pthread_mutex_unlock(&monitor->semaforo_sprima);

}


void monitor_crear_lista_s_prima(monitor* monitor, int tamano){

	// Para empezar a agregar desde 0
	monitor->pos_sprima = 0;

	// Colocar el tamano
	monitor->tamano_sprima = tamano;

	// No hay que hacer free, ya que ahora S apunta a ese arreglo
	// Si hace free, estaria borrando S
	// Simplemente hay que hacer otro malloc y reemplazar el valor del puntero S'

	// Crear el arreglo
	monitor->s_prima = (int*) malloc(sizeof(int) * monitor->tamano_sprima);

}

int comprobar_interseccion_no_vacia(monitor* monitor){
	return monitor->interseccion_no_vacia;
}


void monitor_termine_de_procesar_una_sublista_k(monitor* monitor, lista* S, int id_hilo){

	// Termino uno mas
	pthread_mutex_lock(&monitor->semaforo_sublistak);

	// Incrementar numero de hilos que han terminado
	monitor->cuantos_han_terminado++;
	
	// Si todos terminaron
	if(monitor->cuantos_han_terminado == monitor->cuantos_hilos){

		// Volver a 0
		monitor->cuantos_han_terminado = 0;

		int i;

		// Si la lista de interseccion tiene elementos
		if(monitor->pos_sprima > 0){
			// Preparar la siguiente lista
			monitor->lista_actual++;

			// Eliminar de memoria los contenidos anteriores de la lista S
			free(S->num);

			// Pasar los contenidos de S' a la lista S
			S->num = monitor->s_prima;
			S->tamano = monitor->pos_sprima;

			// Crear una nueva lista S' vacia, del mismo tamano que la anterior
			// (el tamano ira variando con realloc, ya que la lista de interseccion
			// no siempre tiene un tamano menor a la interseccion anterior)
			monitor_crear_lista_s_prima(monitor, monitor->pos_sprima);
		} else {

			// Si no tiene elementos, indicar que esta vacia
			monitor->interseccion_no_vacia = 0;
		}


		// Signal la variable de condicion
		pthread_cond_broadcast(&monitor->todos_terminaron);

		
	} else {
		// Si aun no todos han terminado, esperar
		pthread_cond_wait(&monitor->todos_terminaron, &monitor->semaforo_sublistak);
	}


	pthread_mutex_unlock(&monitor->semaforo_sublistak);
}



int quedan_listas(monitor* monitor){
	if(monitor->lista_actual < monitor->cuantas_listas){
		return 1;
	}
	return 0;
}

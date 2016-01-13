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
	monitor->interseccion_vacia = 0;

	pthread_mutex_init(&monitor->semaforo_sublistak, NULL);
	pthread_mutex_init(&monitor->semaforo_sprima, NULL);

	pthread_cond_init(&monitor->todos_terminaron, NULL);

	pthread_mutex_unlock(&monitor->semaforo_sublistak);
	pthread_mutex_unlock(&monitor->semaforo_sprima);


}


void agregar_elemento_sprima(monitor* monitor, int numero){

	pthread_mutex_lock(&monitor->semaforo_sprima);

	monitor->s_prima[monitor->pos_sprima] = numero;
	monitor->pos_sprima++;

	printf("POS S prima es %d\n", monitor->pos_sprima);

	pthread_mutex_unlock(&monitor->semaforo_sprima);

}


void monitor_crear_lista_s_prima(monitor* monitor){

	pthread_mutex_lock(&monitor->semaforo_sprima);

	// Si ya se ha creado una lista, soltar el mutex, y salir de la funcion
	if(monitor->s_prima != NULL){
		pthread_mutex_unlock(&monitor->semaforo_sprima);
		return;
	}

	monitor->tamano_sprima = monitor->pos_sprima;	
	monitor->pos_sprima = 0;
	monitor->s_prima = (int*) malloc(sizeof(int) * monitor->tamano_sprima);

	printf("Se creo una nueva matriz S prima con tamano %d\n", monitor->tamano_sprima);

	pthread_mutex_unlock(&monitor->semaforo_sprima);

}

int monitor_termine_de_procesar_una_sublista_k(monitor* monitor, lista* S, int id_hilo){

	int i;

	// Incrementar (usando mutex) una variable que sirva para ver cuantos han terminado
	// Cuando todos terminan, se hace S=S'
	// Se verifica cual es el tamano de S, si es 0, retorna 0

	//printf("(ID=%d) mutex lock A\n", id_hilo);
	pthread_mutex_lock(&monitor->semaforo_sublistak);

	//printf("(ID=%d) Atraveso el mutex (creado en A)\n", id_hilo);

	monitor->cuantos_han_terminado++;

	// Si ya todos terminaron
	if(monitor->cuantos_han_terminado == monitor->cuantos_hilos){
		//printf("(ID=%d) Todos terminaron\n", id_hilo);
		// Liberar la memoria de S
		free(S->num);

		// Crear nuevos numeros para la lista S
		S->num = (int*) malloc(sizeof(int) * monitor->pos_sprima);
		S->tamano = monitor->pos_sprima;

		for(i=0; i<monitor->pos_sprima; i++){
			S->num[i] = monitor->s_prima[i];
		}

		// La interseccion tiene longitud 0?
		if(monitor->pos_sprima == 0){
			monitor->interseccion_vacia = 1;
			//printf("(ID=%d) mutex unlock B\n", id_hilo);
			pthread_cond_broadcast(&monitor->todos_terminaron);
			pthread_mutex_unlock(&monitor->semaforo_sublistak);
			return 0;
		}

		monitor->s_prima = NULL;

		// Crear una nueva S'
		monitor_crear_lista_s_prima(monitor);
		

		monitor->lista_actual++;
		monitor->cuantos_han_terminado = 0;

		//printf("(ID=%d) broadcast C\n", id_hilo);
		pthread_cond_broadcast(&monitor->todos_terminaron);

		//printf("(ID=%d) mutex unlock D\n", id_hilo);
		pthread_mutex_unlock(&monitor->semaforo_sublistak);
		return 1;
	}
	else{
		//printf("(ID=%d) Aun no todos terminan\n", id_hilo);

		// Si la interseccion es vacia, quiere decir que un hilo ya aborto la ejecucion
		// Por lo tanto nunca van a poder "terminar todos", por lo que produciria deadlock
		// si no se pone
		if(monitor->interseccion_vacia == 1){
			pthread_cond_broadcast(&monitor->todos_terminaron);
			pthread_mutex_unlock(&monitor->semaforo_sublistak);
			return 0;
		}
	}

	//printf("(ID=%d) mutex unlock E (cond wait)\n", id_hilo);
	pthread_cond_wait(&monitor->todos_terminaron, &monitor->semaforo_sublistak);
	//printf("(ID=%d) Salir del cond wait F\n", id_hilo);

	pthread_mutex_unlock(&monitor->semaforo_sublistak);

	return 1;

}



int quedan_listas(monitor* monitor){
	if(monitor->lista_actual < monitor->cuantas_listas){
		return 1;
	}
	return 0;
}

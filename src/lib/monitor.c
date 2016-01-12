#include "monitor.h"

void inicializar_monitor(monitor* monitor, int cuantos_hilos, int cuantas_listas){

	monitor->cuantos_hilos = cuantos_hilos;
	monitor->cuantas_listas = cuantas_listas;
	monitor->lista_actual = 1;
	monitor->pos_sprima = 0;

	pthread_mutex_unlock(&monitor->semaforo_s);
	pthread_mutex_unlock(&monitor->semaforo_sprima);


}


void agregar_elemento_sprima(monitor* monitor, int numero){

	pthread_mutex_lock(&monitor->semaforo_sprima);

	monitor->s_prima[monitor->pos_sprima] = numero;
	monitor->pos_sprima++;

	pthread_mutex_unlock(&monitor->semaforo_sprima);

}


void monitor_crear_lista_s_prima(monitor* monitor, int tamano_lista){

}

int monitor_termine_de_procesar_una_sublista_k(lista* S){

	// Incrementar (usando mutex) una variable que sirva para ver cuantos han terminado
	// Cuando todos terminan, se hace S=S'
	// Se verifica cual es el tamano de S, si es 0, retorna 0

	return 0;

}


int quedan_listas(monitor* monitor){
	if(monitor->lista_actual < monitor->cuantas_listas){
		return 1;
	}
	return 0;
}

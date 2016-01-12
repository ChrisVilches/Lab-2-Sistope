#include "monitor.h"

void inicializar_monitor(monitor* monitor, int cuantos_hilos, int cuantas_listas){

	monitor->cuantos_hilos = cuantos_hilos;
	monitor->cuantas_listas = cuantas_listas;
	monitor->lista_actual = 1;
	monitor->pos_sprima = 0;

	pthread_mutex_unlock(&monitor->semaforo_s);
	pthread_mutex_unlock(&monitor->semaforo_sprima);


}


void agregar_elemento_sprima(int numero){

	pthread_mutex_lock(&monitor->semaforo_sprima);

	s_prima.num[pos_sprima] = numero;
	pos_sprima++;

	pthread_mutex_unlock(&monitor->semaforo_sprima);

}
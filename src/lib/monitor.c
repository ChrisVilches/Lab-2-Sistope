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


int quedan_listas(monitor* monitor){
	if(monitor->lista_actual < monitor->cuantas_listas){
		return 1;
	}
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "grupohilo.h"
#include "monitor.h"


struct argumento{

	grupohilo* ptr_grupohilo;
	int id_hilo;

};


void inicializar_grupohilo(grupohilo* grupohilo, char* nombre_archivo, int threads_por_equipo){

	grupohilo->num_threads = threads_por_equipo; 
    grupohilo->hilos = (pthread_t*) malloc(sizeof(pthread_t) * grupohilo->num_threads);
    grupohilo->tiempo_hebra = (double*) malloc(sizeof(double) * grupohilo->num_threads);
    
    inicializar_listas(&grupohilo->conjunto_listas, nombre_archivo);

}


// Hebra que se dedica a cooperar con su grupo, a intersectar las listas
void* hebra_intersecta(void* arg){
	clock_t begin, end;
	double time_spent;
	struct argumento arg1 = *((struct argumento*) arg);
	int id_hilo = arg1.id_hilo;
	grupohilo* grupohilo = arg1.ptr_grupohilo;

	// Esta hebra tiene una ID = id_hilo
	// Y un puntero a su grupo = grupohilo
	// para poder acceder a la lista de su grupo

	// Comenzar a contar el tiempo
	begin = clock();
	
	//
	// Aca va el codigo al cual se le quiere medir el tiempo
	//

	// Detener la cuenta del tiempo
	end = clock();

	// Obtener cuanto tiempo tardo
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	// Guardar el tiempo en el arreglo global
	grupohilo->tiempo_hebra[id_hilo] = time_spent;


}


void intersectar_listas(grupohilo* grupohilo, int* mejor_hebra, double* promedio_tiempos){

	int i;
	int min;
	double suma;

	// Crear las ID para cada hilo y ademas guardar el puntero al grupo
	struct argumento* argumento = (struct argumento*) malloc(sizeof(struct argumento) * grupohilo->num_threads);

	// Enumerar cada hilo
	for(i=0; i<grupohilo->num_threads; i++){
		argumento[i].id_hilo = i;
		argumento[i].ptr_grupohilo = grupohilo;
	}

	// Crear hilos
	for(i=0; i<grupohilo->num_threads; i++){
		pthread_create(&grupohilo->hilos[i], NULL, hebra_intersecta, &argumento[i]);
	}

	// Juntar hilos
	for(i=0; i<grupohilo->num_threads; i++){
		pthread_join(grupohilo->hilos[i], NULL);
	}

	// Obtener cual fue la mejor hebra
	min = 0;

	for(i=1; i<grupohilo->num_threads; i++){
		if(grupohilo->tiempo_hebra[i] < grupohilo->tiempo_hebra[min]){
			min = i;
		}
	} 

	// Obtener promedio
	suma = 0;
	for(i=0; i<grupohilo->num_threads; i++){
		suma += grupohilo->tiempo_hebra[i];
	} 

	*promedio_tiempos = suma/grupohilo->num_threads;
	*mejor_hebra = min;


}

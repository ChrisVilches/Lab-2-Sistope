#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <pthread.h>
#include "lib/listas.h"
#include "lib/grupohilo.h"
#include "lib/lib.h"


/*
*	Variables globales
*/
int cantidad_equipos = 0;
int threads_por_equipo = 0;
char* nombre_archivo;
double* tiempos_equipos;	// Sirve para que cada equipo guarde sus tiempos (para que luego el main los vea todos)
int* mejor_hebra;			// Cada equipo almacenara en este arreglo (posicion del equipo), la mejor hebra de su equipo
double* promedios_tiempos;	// Cada equipo guarda aca el promedio de los tiempos de todas sus hebras


/*
*	Este procedimiento corresponde a lo que hace un equipo
*	Cada equipo se representa por un hilo
*/
void* ejecutar_hilo_equipo(void* arg){

	// Obtener la ID del equipo, pasada desde el argumento
	int id_equipo = *((int*) arg);
	clock_t begin, end;
	double time_spent;
	int id_mejor_hebra_intersecta;
	double promedio_tiempos;

	// Se crea el grupo
	grupohilo equipo;
	inicializar_grupohilo(&equipo, nombre_archivo, threads_por_equipo);


	// Comenzar a contar el tiempo
	begin = clock();

	// Comenzar a intersectar las listas
	// Dado que la funcion intersectar_listas provee los mecanismos de Join, y crear hebras
	// no es necesario hacerlo aca
	intersectar_listas(&equipo, &id_mejor_hebra_intersecta, &promedio_tiempos);		

	// Detener la cuenta del tiempo
	end = clock();

	// Obtener cuanto tiempo tardo
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	// Guardar el tiempo en el arreglo global
	tiempos_equipos[id_equipo] = time_spent;

	// Guardar en el arreglo cual fue la mejor hebra de este equipo
	mejor_hebra[id_equipo] = id_mejor_hebra_intersecta;

	// Guardar promedio
	promedios_tiempos[id_equipo] = promedio_tiempos;

}

int main(int argc, char** argv){
	
	char option;
	int i;
	pthread_t* grupos_hilos;
	int* id_equipos;

	int flagarg1 = 0;
	int flagarg2 = 0;
	int flagarg3 = 0;

	// Obtener opciones

	while ((option = getopt(argc, argv, "g:h:i:")) != -1)
	    switch (option)
	    {
		    case 'g':
			    cantidad_equipos = atoi(optarg);
			    flagarg1++;
			    break;
		    case 'h':
			    threads_por_equipo = atoi(optarg);
			    flagarg2++;
			    break;
		    case 'i':
			    nombre_archivo = optarg;
			    flagarg3++;
			    break;		    
		    default:
		    	abort();
	    }

	// Validar opciones
	if(flagarg1 == 0 || flagarg2 == 0 || flagarg3 == 0){
		printf("Argumentos invalidos\n");
		exit(1);
	}

	// Crear el arreglo para que cada equipo almacene sus tiempos
	tiempos_equipos = (double*) malloc(sizeof(double) * cantidad_equipos);

	// Arreglo donde cada equipo almacenara su mejor hebra
	mejor_hebra = (int*) malloc(sizeof(int) * cantidad_equipos);

	// Promedios de las hebras de cada grupo (tiempos)
	promedios_tiempos = (double*) malloc(sizeof(double) * cantidad_equipos);

	// Crear equipos, cada equipo es un hilo
	grupos_hilos = (pthread_t*) malloc(sizeof(pthread_t) * cantidad_equipos);

	// Crear las IDs de los equipos
	id_equipos = (int*) malloc(sizeof(int) * cantidad_equipos);

	// Enumerar las ID desde 0 hasta N-1 (cantidad de equipos)
	for(i=0; i<cantidad_equipos; i++){
		id_equipos[i] = i;
	}

	// Crear todos los hilos para cada grupo
	for(i=0; i<cantidad_equipos; i++){
		pthread_create(&grupos_hilos[i], NULL, ejecutar_hilo_equipo, &id_equipos[i]);
	}
	
	/*
	*
	*	(En este punto, la competencia ya empezo)
	*
	*/

	// Juntar todos los hilos (cada grupo)
	for(i=0; i<cantidad_equipos; i++){
		pthread_join(grupos_hilos[i], NULL);
	}

	printf("Fin de la competencia\n");

	for(i=0; i<cantidad_equipos; i++){
		printf("Equipo #%d: %f (mejor hebra id %d, promedio tiempo %f)\n", i, tiempos_equipos[i], mejor_hebra[i], promedios_tiempos[i]);
	}


	// Liberar memoria

	
	return 0;

}
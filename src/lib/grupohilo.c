#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "grupohilo.h"
#include "lib.h"


struct argumento{

	grupohilo* ptr_grupohilo;
	int id_hilo;

};


void inicializar_grupohilo(grupohilo* grupohilo, char* nombre_archivo, int threads_por_equipo){

	grupohilo->num_threads = threads_por_equipo; 
    grupohilo->hilos = (pthread_t*) malloc(sizeof(pthread_t) * grupohilo->num_threads);
    grupohilo->tiempo_hebra = (double*) malloc(sizeof(double) * grupohilo->num_threads);
    

    leer_listas(grupohilo, nombre_archivo);

    inicializar_monitor(&grupohilo->monitor, grupohilo->num_threads, grupohilo->cuantas_listas);

}


// Hebra que se dedica a cooperar con su grupo, a intersectar las listas
void* hebra_intersecta(void* arg){

	// Datos para calcular el tiempo
	clock_t begin, end;
	double time_spent;	

	// Se pueden sacar los argumentos de la estructura arg1
	struct argumento arg1 = *((struct argumento*) arg);

	// Extraer argumentos
	int id_hilo = arg1.id_hilo;
	grupohilo* grupohilo = arg1.ptr_grupohilo;
	monitor* monitor = &grupohilo->monitor;

	// La lista S mas corta
	lista* S;

	// Comenzar a contar el tiempo
	begin = clock();
	
	S = &grupohilo->conjunto_listas[0];
	/*
	

	while(QUEDEN LISTAS (NO CUENTA LA PRIMERA, QUE ES LA MAS CORTA)){

		(El monitor es quien proporciona las listas, es decir mantiene registro de cuantas quedan, etc)
		(Por lo tanto, si sabe esa informacion, entonces tambien puede crear la S prima vacia cuando sea necesario)
		(Aca el monitor debe crear una lista vacia S prima, la cual debe ser accedida usando MUTEX por cada hebra)
		(La lista S prima tiene una longitud maxima de S (la mas corta), ya que es una interseccion... no puede ser mas larga)

		lista* K = la siguiente

		Desde = Obtener rango K/P

		Hasta = Obtener rango K/P

		Si "Desde" esta dentro de la lista, pero "Hasta" esta fuera, entonces truncar Hasta para que sea el ultimo elemento de la lista
		// Si ambos estan fuera, podria ser el caso en que todos los elementos de la lista estan siendo ocupados
		// por una hebra, y a esta hebra no le toca ningun numero de la lista


		if(Si "Desde" y "Hasta" estan dentro){

			quicksort la sublista K usando el rango como limites

			for(i=0; i<|S|; i++){
				elemento_S = S[i]

				// Si lo encontro
				if(K.busquedabinaria(elemento_S) != -1){

					(lock)
					S_prima.agregar_acceso_exclusivo(elemento_S)
					(unlock)
				}
			}


		} else {
			// No esta dentro de la lista
			// Si hay P = 100 hilos
			// Y K=5
			// K/P = [5/100] = 1
			// Ocupando la ID de la hebra se puede obtener (multiplicando) el rango

			// Ahora un caso donde hay mas elementos que hebras
			// K = 1000
			// P = 3
			// K/P = [1000/3] = 334
			// La hebra id=0 --> 0*334 hasta 0*334 + 333 = 0 hasta 333
			// La hebra id=1 --> 1*334 hasta 1*334 + 333 = 334 hasta 667
			// La hebra id=2 --> 2*334 hasta 2*334 + 333 = 668 hasta 1001

			// Un caso medio
			// K = 25
			// P = 4
			// K/P = [25/4] = [6.25] = 7
			// La hebra id=0 --> 0*7 hasta 0*7 + 7 - 1 = 0 hasta 6
			// La hebra id=1 --> 1*7 hasta 1*7 + 7 - 1 = 7 hasta 13
			// La hebra id=2 --> 2*7 hasta 2*7 + 7 - 1 = 14 hasta 20
			// La hebra id=3 --> 3*7 hasta 3*7 + 7 - 1 = 21 hasta 27
		}


		wait(esperar si aun las otras hebras no han terminado con su procesamiento)

		
		(Aca el monitor tiene que liberar la memoria de S y hacer S=S prima)
		(Aca se puede dar cuenta si la lista S es vacia, en ese caso, se debe detener la ejecucion del algoritmo)

	}
*/
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

	// Ordenar listas de tamano menor a tamano mayor (no ordena los elementos de las listas,
	// si no que solamente la posicion de las listas en el arreglo, usando el tamano de la
	// lista como criterio)
	quicksort_arreglo_listas(grupohilo->conjunto_listas, grupohilo->cuantas_listas);

	// Enumerar cada hilo
	for(i=0; i<grupohilo->num_threads; i++){
		argumento[i].id_hilo = i;
		argumento[i].ptr_grupohilo = grupohilo;
	}

	printf("Monitor. Cantidad hilos %d, cantidad listas %d, lista actual %d\n", grupohilo->monitor.cuantos_hilos, grupohilo->monitor.cuantas_listas, grupohilo->monitor.lista_actual);

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

// Imprime una lista
void mostrarlista(grupohilo* grupohilo, int indice){
	int i;
	for(i=0; i<grupohilo->conjunto_listas[indice].tamano; i++){
		printf("%d ", grupohilo->conjunto_listas[indice].num[i]);
	}
	printf("\n");
}

// Crea las listas
void leer_listas(grupohilo* grupohilo, char* nombre_archivo){

	FILE* fp;
	char line[4096];
	char line2[4096];
	int longitud_lista;
	char* token;
	int cantidad_listas = 0;
	int i = 0;
	int j;

	fp = fopen(nombre_archivo, "r");

	// Validar el archivo
	if(fp == NULL){
		printf("Archivo no encontrado: %s\n", nombre_archivo);
		abort();
	}

	// Contar la cantidad de lineas que tiene el archivo
	while (fgets(line, sizeof(line), fp) != NULL){
		cantidad_listas++;
	}
	
	// Crear el conjunto de listas
	grupohilo->conjunto_listas = (lista*) malloc(sizeof(lista) * cantidad_listas);
	grupohilo->cuantas_listas = cantidad_listas;

	// Volver al inicio del archivo
	rewind(fp);

	// Crear las listas
	while (fgets(line, sizeof(line), fp) != NULL){

		j=1;

		// Copiar la linea
		strcpy(line2, line);

		// Contar cuantos numeros tiene la lista
		longitud_lista = 0;
		token = strtok(line, " ");
		while(token != NULL && token[0] >= '0' && token[0] <= '9') {			
			token = strtok(NULL, " ");
			longitud_lista++;
		}

		// Crear una nueva lista del tamano leido
		lista nueva;
		nueva.tamano = longitud_lista;
		nueva.num = (int*) malloc(sizeof(int) * longitud_lista);

		// Cortar nuevamente la string, para poder agregar los numeros
		token = strtok(line2, " ");
		nueva.num[0] = atoi(token);

		while(token != NULL) {	
			token = strtok(NULL, " ");	
			if(token != NULL && token[0] >= '0' && token[0] <= '9'){
				nueva.num[j] = atoi(token);
			} else {
				break;
			}	

			j++;
		}

		// Guardar esta lista en la posicion adecuada
		// en el conjunto de listas
		grupohilo->conjunto_listas[i] = nueva;
		i++;
	}
	fclose(fp);	
}

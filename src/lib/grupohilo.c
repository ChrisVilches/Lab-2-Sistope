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

	int i;

	// Datos para calcular el tiempo
	clock_t begin, end;
	double time_spent;	

	// Se pueden sacar los argumentos de la estructura arg1
	struct argumento arg1 = *((struct argumento*) arg);

	// Extraer argumentos
	int id_hilo = arg1.id_hilo;
	grupohilo* grupohilo = arg1.ptr_grupohilo;
	monitor* monitor = &grupohilo->monitor;

	// Las listas S y K
	lista* S;
	lista* K;

	// Rangos "desde" y "hasta" para calcular cual sera la sublista K de esta hebra
	int desde;
	int hasta;
	int k_dividido_p;
	

	// Comenzar a contar el tiempo
	begin = clock();
	
	// Obtener la lista inicial, la mas corta
	S = &grupohilo->conjunto_listas[0];

	while(quedan_listas(monitor))
	{

		// Preguntar al monitor cual es la siguiente lista K a examinar
		K = &grupohilo->conjunto_listas[monitor->lista_actual];

		// Calcular K/P
		k_dividido_p = K->tamano/grupohilo->num_threads;

		// Si la division no es perfecta, significa que se aproxima al numero siguiente
		// Esto es el equivalente a obtener [K/P]
		if(K->tamano % grupohilo->num_threads != 0){
			k_dividido_p++;
		}

		// Calcular rangos
		desde = (id_hilo * k_dividido_p);
		hasta = desde + k_dividido_p - 1;

		// Si "desde" esta dentro de la lista K, pero "hasta" esta fuera, entonces truncar "hasta"
		if(desde < K->tamano && !(hasta < K->tamano)){
			hasta = K->tamano-1;
		} 

		// Si "desde" y "hasta" estan dentro de la lista, entonces tienen elementos con cuales trabajar
		if(desde < K->tamano && hasta < K->tamano){

			// Ordenar la sublista k
			quicksort_lista_limites(K, desde, hasta);

			// Para todo elemento de S
			for(i=0; i<S->tamano; i++){				

				// Buscar si existe S[i] en la lista K
				if(existe_elemento_en_busquedabinaria(S->num[i], K)){
					// Si esta, entonces agregarlo a la lista S'
					agregar_elemento_sprima(monitor, S->num[i]);
					printf("SI NO SE HA CREADO LA LISTA S PRIMA, DA SEGMENTATION FAULT!!!\n");
				}
			}


		}

		printf("Soy el hilo id=%d, mi rango es desde %d hasta %d\n", id_hilo, desde, hasta);
		break;
	}


	/*
	while(QUEDEN LISTAS (NO CUENTA LA PRIMERA, QUE ES LA MAS CORTA)){

		(El monitor es quien proporciona las listas, es decir mantiene registro de cuantas quedan, etc)
		(Por lo tanto, si sabe esa informacion, entonces tambien puede crear la S prima vacia cuando sea necesario)
		(Aca el monitor debe crear una lista vacia S prima, la cual debe ser accedida usando MUTEX por cada hebra)
		(La lista S prima tiene una longitud maxima de S (la mas corta), ya que es una interseccion... no puede ser mas larga)

		--lista* K = la siguiente

		--Desde = Obtener rango K/P

		--Hasta = Obtener rango K/P

		--Si "Desde" esta dentro de la lista, pero "Hasta" esta fuera, entonces truncar Hasta para que sea el ultimo elemento de la lista
		// Si ambos estan fuera, podria ser el caso en que todos los elementos de la lista estan siendo ocupados
		// por una hebra, y a esta hebra no le toca ningun numero de la lista


		--if(Si "Desde" y "Hasta" estan dentro){

			--quicksort la sublista K usando el rango como limites

			--for(i=0; i<|S|; i++){
			--	elemento_S = S[i]

				// Si lo encontro
				if(K.busquedabinaria(elemento_S) != -1){
*/
					//agregar_elemento_sprima(monitor, 4);

					/*
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

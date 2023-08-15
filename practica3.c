#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <semaphore.h>

#include"estructura.h"

#define THREADS 2

int *numeros;
int cantidad_elementos = 0;
sem_t semaforo;

void merge(int left, int middle, int right) {
    	int size1 = middle - left + 1;
    	int size2 = right - middle;

    	int *temp = (int *)malloc((size1 + size2) * sizeof(int));

    	if (temp == NULL) {
        	fprintf(stderr, "Error al asignar memoria\n");
        	return;
    	}

    	int i = left, j = middle + 1, k = 0;

    	while (i <= middle && j <= right) {
        	if (numeros[i] <= numeros[j])
            		temp[k++] = numeros[i++];
        	else
            		temp[k++] = numeros[j++];
    	}

    	while (i <= middle)
        	temp[k++] = numeros[i++];

    	while (j <= right)
        	temp[k++] = numeros[j++];

    	for (k = 0; k < size1 + size2; k++)
        	numeros[left + k] = temp[k];

    	free(temp);
}


void* mergeSort(void* arg){
	pthread_t threads[THREADS];
	Arreglo array = *((Arreglo*)arg);
    	int first = array.first;
    	int last = array.last;
    	if (first < last){
        	int middle = first+(last-first)/2;
        	Arreglo arr1 = {first, middle};
		Arreglo arr2 = {middle+1, last};

        	pthread_create(&threads[0], NULL, mergeSort, &arr1);
        	pthread_create(&threads[1], NULL, mergeSort, &arr2);
        	pthread_join(threads[0], NULL);
        	pthread_join(threads[1], NULL);


		sem_wait(&semaforo);
        	merge(first, middle, last);
		sem_post(&semaforo);
    	}
	return NULL;
}



int main(int argc, char *argv[]){

	// Inicializar el semáforo con un valor inicial
    	if (sem_init(&semaforo, 0, 1) == -1) {
        	perror("Error al inicializar el semáforo");
        	exit(EXIT_FAILURE);
    	}

    	if(argc != 2){
		fprintf(stderr, "Uso del programa: mergesort <nombre_archivo:string>");
		exit(EXIT_FAILURE);
	}
	FILE* ptr;
	char *filename = argv[1];
    	ptr = fopen(filename, "r");

    	if (NULL == ptr) {
        	printf("Error al abrir el achivo \n");
    	}

    	char linea[1024];
    	if (fgets(linea, sizeof(linea), ptr)) {
        	char *token = strtok(linea, ",");
        	while (token != NULL) {
         		cantidad_elementos++;
            		token = strtok(NULL, ",");
        	}
    	}
	fseek(ptr, 0, SEEK_SET);

	numeros = (int*)malloc(cantidad_elementos * sizeof(int));
    	if (numeros == NULL) {
        	fprintf(stderr, "Error al asignar memoria\n");
       	 	fclose(ptr);
        return 1;
    	}

	for (int i = 0; i < cantidad_elementos; i++) {
        	fscanf(ptr, "%d,", &numeros[i]);
    	}

	fclose(ptr);

	printf("El arreglo posee %d elementos\n", cantidad_elementos);

	Arreglo arreglo = {0, cantidad_elementos-1};

	pthread_t tid;

    	pthread_create(&tid, NULL, mergeSort, &arreglo);
    	pthread_join(tid, NULL);

	sem_destroy(&semaforo);

	printf("Los elementos ordenados de manera ascendente son: \n");

	for(int i = 0; i < cantidad_elementos; i++){
		printf("%d) %d\n",i+1 ,numeros[i]);
	}
   	return 0;
}

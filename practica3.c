#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#include"estructura.h"

#define THREADS 2

int *numeros;
int contador = 0;

void merge(int left, int middle, int right){
    	int i, j, k;
    	int n1 = middle - left + 1;
    	int n2 =  right - middle;

    	//int low[n1], high[n2];
	int *low = (int *)malloc(n1 * sizeof(int));
	int *high = (int *)malloc(n2 * sizeof(int));

	if(low == NULL || high == NULL){
		fprintf(stderr, "Error al asignar memoria\n");
	}

	for (i = 0; i < n1; i++)
        	low[i] = numeros[left + i];
    	for (j = 0; j < n2; j++)
        	high[j] = numeros[middle+ 1+ j];
    	i = 0;
    	j = 0;
    	k = left;
    	while (i < n1 && j < n2){
        	if (low[i] <= high[j]){
            		numeros[k] = low[i];
            		i++;
       	 	}
        	else{
            		numeros[k] = high[j];
            		j++;
        	}
        	k++;
    	}

    	while (i < n1){
        	numeros[k] = low[i];
        	i++;
        	k++;
    	}

    	while (j < n2){
        	numeros[k] = high[j];
        	j++;
        	k++;
    	}

	free(low);
	free(high);
}



void* mergeSort(void* arg){
	pthread_t threads[THREADS];
	Arreglo array = *((Arreglo*)arg);
    	int left = array.left;
    	int right = array.right;
	printf("Left: %d\n", left);
	printf("Right: %d\n", right);
    	if (left < right){
        	int middle = left+(right-left)/2;
		printf("Middle: %d\n", middle);
        	//pthread_t threads[THREADS];
        	Arreglo a1 = {left, middle};
		Arreglo a2 = {middle+1, right};
        	pthread_create(&threads[0], NULL, mergeSort, &a1);
        	pthread_create(&threads[1], NULL, mergeSort, &a2);

        	pthread_join(threads[0], NULL);
        	pthread_join(threads[1], NULL);

        	merge(left, middle, right);
    	}
	return NULL;
}



int main(int argc, char *argv[]){
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
        		//numeros[contador] = atoi(token);
         		contador++;
            		token = strtok(NULL, ",");
        	}
    	}
	fseek(ptr, 0, SEEK_SET); // Regresar al inicio del archivo

	numeros = (int*)malloc(14 * sizeof(int));
    	if (numeros == NULL) {
        	fprintf(stderr, "Error al asignar memoria\n");
       	 	fclose(ptr);
        return 1;
    	}

	for (int i = 0; i < contador; i++) {
        	fscanf(ptr, "%d,", &numeros[i]);
    	}

	fclose(ptr);

	Arreglo arr = {0, contador-1};

	pthread_t tid;

    	pthread_create(&tid, NULL, mergeSort, &arr);
    	pthread_join(tid, NULL);

	for(int i = 0; i < contador; i++){
		printf("%d\n", numeros[i]);
	}

	printf("Cantidad elementos: %d\n", contador);

    	//fclose(ptr);
   	return 0;
}

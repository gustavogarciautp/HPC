#include <pthread.h>
#include <chrono>
#include <iostream>

int  **mt1, **mt2, **mt3, **mt4;
int tam;


void llenar(int ** mt){
	for (int i=0; i<tam; i++){
		for (int j=0; j< tam; j++){
			mt[i][j]= 1+rand() % (20);  // de 1 a 20
		}			
	}
}

void imprimir(int **mt){
	for (int i=0; i<tam; i++){
		printf("\n");
		for (int j=0; j< tam; j++){
			printf("%i\t", mt[i][j]);
		}			
	}
	printf("\n------------------------------------------------------------------------------------------------------------------\n");
}

void * multiplicacion(void *threadid){
	long i= (long) threadid;
	for (int j=0; j<tam; j++){
		mt3[i][j]=0;
		for(int k=0; k<tam; k++){
			mt3[i][j]+= mt1[i][k]*mt4[j][k];
		}
	}
	pthread_exit(NULL);
}


void transpuesta(void){
	for (int i=0; i<tam; i++){
		for (int j=0; j<tam; j++){
			mt4[j][i]= mt2[i][j];
		}
	}
}


/*
void imprimir_t(int **mt){
	for (int i=0; i<tam; i++){
		printf("\n");
		for (int j=0; j< tam; j++){
			printf("%i\t", mt[j][i]);
		}			
	}
	printf("\n------------------------------------------------------------------------------------------------------------------\n");
}
*/	
int main(int argc, char *argv[]){
	if (argc==2){
		tam= atoi(argv[1]);

		mt1=(int **)malloc(sizeof(int *)*tam); //Reservar memoria para filas
		mt2=(int **)malloc(sizeof(int *)*tam);
		mt3=(int **)malloc(sizeof(int *)*tam);
		mt4=(int **)malloc(sizeof(int *)*tam);
		for (int i=0; i<tam; i++){
			mt1[i]= (int *)malloc(sizeof(int)*tam);
			mt2[i]= (int *)malloc(sizeof(int)*tam);
			mt3[i]= (int *)malloc(sizeof(int)*tam);
			mt4[i]= (int *)malloc(sizeof(int)*tam); //mt4 es la transpuesta de mt2
		}
		llenar(mt1);
		llenar(mt2);
		transpuesta(mt2);
		
		pthread_t threads[tam];
		int rc, t;
		long taskids[tam];
		
		auto start = std::chrono::system_clock::now();

		for (t=0; t< tam; t++){
			taskids[t] = t;
			rc = pthread_create(&threads[t], NULL, multiplicacion, (void *) taskids[t]);
			if (rc) {
			   exit(-1);
			}
		}
		for (int i=0; i< tam; i++){
			pthread_join (threads[i], NULL );
		}
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<float,std::ratio<1>> duration = end - start;

		printf("%d\t%.3f\n", atoi(argv[1]), duration.count());

		//printf("%d\n", mt3[tam-1][tam-1]);
		//imprimir(mt1);
		//imprimir(mt2);
		//imprimir(mt3);
		//imprimir_t(mt2);

		free(mt1);
		free(mt2);
		free(mt3);
		free(mt4);
	}
	else{
		printf("El numero de argumentos es incorrecto");
	}
	pthread_exit(NULL);
	return 0;
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int  **mt1, **mt2, **mt3;
int tam;

void llenar(int ** mt){
	for (int i=0; i<tam; i++){
		for (int j=0; j< tam; j++){
			mt[i][j]= 1+rand() % (20);  // de 0 a 20
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

//Cada hilo calcula una unica columna
void * multiplicacion(void *threadid){
	long j= (long) threadid;;
	for (int i=0; i<tam; i++){
		mt3[i][j]=0;
		for(int k=0; k<tam; k++){
			mt3[i][j]+= mt1[i][k]*mt2[k][j];
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if (argc==2){
		tam= atoi(argv[1]);

		double total_time;
		clock_t start, end;

		mt1=(int **)malloc(sizeof(int *)*tam); //Reservar memoria para filas
		mt2=(int **)malloc(sizeof(int *)*tam);
		mt3=(int **)malloc(sizeof(int *)*tam);
		for (int i=0; i<tam; i++){
			mt1[i]= (int *)malloc(sizeof(int)*tam);
			mt2[i]= (int *)malloc(sizeof(int)*tam);
			mt3[i]= (int *)malloc(sizeof(int)*tam);
		}
		llenar(mt1);
		llenar(mt2);
		//imprimir(num_row, num_col, mt1);
		//imprimir(num_row, num_col, mt2); 
		
		pthread_t threads[tam];
		int rc, t;
		long taskids[tam];
	
		start= clock();
		for (t=0; t< tam; t++){
			taskids[t] = t;
		  rc = pthread_create(&threads[t], NULL, multiplicacion, (void *) taskids[t]);
		  if (rc) {
		    //printf("ERROR; return code from pthread_create() is %d\n", rc);
		    exit(-1);
		    }
		  }

		for (int i=0; i< tam; i++){
			pthread_join (threads[i], NULL );
		}
		end= clock();
		//printf("Este es el codigo paralelo\n");
		total_time= ((double) (end-start))/	CLOCKS_PER_SEC;
		printf("%d\t%.3f\n", atoi(argv[1]), total_time);
		free(mt1);
		free(mt2);
		free(mt3);
	}
	else{
		printf("El numero de argumentos es incorrecto");
	}
	pthread_exit(NULL);
	return 0;
}
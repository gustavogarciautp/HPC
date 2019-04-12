#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int  **mt1, **mt2, **mt3;

struct thread_data
{
   int	rows, cols, ini_cols;
};

void llenar(int rows, int cols, int ** mt){
	for (int i=0; i<rows; i++){
		for (int j=0; j< cols; j++){
			mt[i][j]= rand() % (20);  // de 0 a 20
		}			
	}
}

void imprimir(int rows, int cols, int **mt){
	for (int i=0; i<rows; i++){
		printf("\n");
		for (int j=0; j< cols; j++){
			printf("%i\t", mt[i][j]);
		}			
	}
	printf("\n------------------------------------------------------------------------------------------------------------------\n");
}

void * multiplicacion(void *threadarg){
	int rows, cols,ini_cols;
	struct thread_data *my_data;
   	my_data = (struct thread_data *) threadarg;
	rows= my_data->rows;
	cols= my_data->cols;
	ini_cols= my_data->ini_cols;

	//printf("ini_cols %i ini_cols+cols %i \n", ini_cols, ini_cols+cols);

	for (int i=0; i<rows; i++){
		for (int j=ini_cols; j< (ini_cols+cols); j++){
			mt3[i][j]=0;
			for(int k=0; k<rows; k++){
				mt3[i][j]+= mt1[i][k]*mt2[k][j];
			}
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	if (argc==4){
		int num_row= atoi(argv[1]);
		int num_col= atoi(argv[2]);
		int NUM_THREADS= atoi(argv[3]);

		double total_time;
		clock_t start, end;

		mt1=(int **)malloc(sizeof(int *)*num_row); //Reservar memoria para filas
		mt2=(int **)malloc(sizeof(int *)*num_row);
		mt3=(int **)malloc(sizeof(int *)*num_row);
		for (int i=0; i<num_row; i++){
			mt1[i]= (int *)malloc(sizeof(int)*num_col);
			mt2[i]= (int *)malloc(sizeof(int)*num_col);
			mt3[i]= (int *)malloc(sizeof(int)*num_col);
		}
		llenar(num_row, num_col, mt1);
		llenar(num_row, num_col, mt2);
		//imprimir(num_row, num_col, mt1);
		//imprimir(num_row, num_col, mt2); 
		
		pthread_t threads[NUM_THREADS];
		int rc, t, cols;

		struct thread_data thread_data_array[NUM_THREADS];

		cols= num_col/NUM_THREADS;
		//printf("cols ... %i\n", cols);
		int res_cols= num_col % NUM_THREADS;
		int sum_cols=0;
		//printf("num_col %i NUM_THREADS %i res_cols ... %i\n", num_col,NUM_THREADS,res_cols);

		
		for(t=0;t<NUM_THREADS;t++) {
		  //printf("Creating thread %d\n", t);
		
		  thread_data_array[t].rows = num_row;

		  if (res_cols!=0){
		  	thread_data_array[t].ini_cols = sum_cols;
		  	res_cols-=1;
		  	sum_cols+=cols+1;
		  	thread_data_array[t].cols = cols+1;
			}
			else{
				thread_data_array[t].ini_cols = sum_cols;
			  	sum_cols+=cols;
			  	thread_data_array[t].cols = cols;
			}
			//printf("hilos %i res_cols %i sum_cols %i\n", NUM_THREADS, res_cols, sum_cols);
		}
		start= clock();
		for (t=0; t< NUM_THREADS; t++){
		  rc = pthread_create(&threads[t], NULL, multiplicacion, (void *) &thread_data_array[t]);
		  if (rc) {
		    //printf("ERROR; return code from pthread_create() is %d\n", rc);
		    exit(-1);
		    }
		  }
		//imprimir(num_row, num_col, mt3);
		//printf("hola2");
		for (int i=0; i< NUM_THREADS; i++){
			pthread_join (threads[i], NULL );
		}
		end= clock();
		total_time= ((double) (end-start))/	CLOCKS_PER_SEC;
		printf("%d\t%.3f\n", atoi(argv[1]), total_time);
		//imprimir(num_row, num_col, mt3);
		//printf("hola1");
		free(mt1);
		free(mt2);
		free(mt3);
	}
	else{
		printf("El numero de argumentos es incorrecto");
	}
	pthread_exit(NULL);
}
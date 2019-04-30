#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int **mt1, **mt2, **mt3;


void llenar(int rows, int cols, int **mt){
	for (int i=0; i<rows; i++){
		for (int j=0; j< cols; j++){
			mt[i][j]= rand() % (20);  // de 0 a 20
		}			
	}
}

void imprimir(int rows, int cols, int ***mt){
	for (int i = 0; i < rows; i++){
		printf("\n");
		for (int j = 0; j < cols; j++){
			printf("%d\t", (*mt)[i][j]);
		}			
	}
	printf("\n");
}

void multiplicacion(int rows, int cols, int ***mt1, int ***mt2, int ***mt3){
	for (int i = 0; i < rows; i++){
		pid_t subprocess = fork();

		if (subprocess < 0) {
				fprintf(stderr, "No se pudo crear el subproceso\n");
				exit(EXIT_FAILURE);
		} else if (subprocess == 0) {
			for (int j = 0; j < cols; j++) {
				(*mt3)[i][j] = 0;

				for(int k = 0; k < cols; k++) {
					(*mt3)[i][j] += (*mt1)[i][k]*(*mt2)[k][j];
					printf("%d\t%d\t%d\n", (*mt1)[i][k], (*mt2)[k][j], (*mt3)[i][j]);
				}
			}
			_exit(0);
		} else {
			continue;
		}
	}
}

int main(int argc, char *argv[]){
	if (argc==3){

		double total_time;
		clock_t start, end;

		int num_row= atoi(argv[1]);
		int num_col= atoi(argv[2]);


		mt1 = (int **)malloc(sizeof(int *)*num_row);
		mt2 = (int **)malloc(sizeof(int *)*num_row);
		mt3 = (int **)malloc(sizeof(int *)*num_row);
		
		for (int i=0; i<num_row; i++){
			mt1[i]= (int *)malloc(sizeof(int)*num_col);
			mt2[i]= (int *)malloc(sizeof(int)*num_col);
			mt3[i]= (int *)malloc(sizeof(int)*num_col);
		}
		llenar(num_row, num_col, mt1);
		llenar(num_row, num_col, mt2);


		start= clock();
		multiplicacion(num_row, num_col, &mt1, &mt2, &mt3);
		end=clock();

		sleep(20);


		imprimir(num_row, num_col, &mt1);
		printf("\n");
		imprimir(num_row, num_col, &mt2);
		printf("\n");
		imprimir(num_row, num_col, &mt3);
		printf("\n");

		total_time= ((double) (end-start))/	CLOCKS_PER_SEC;
		printf("%d\t%.3f\n", atoi(argv[1]), total_time);

		free(mt1);
		free(mt2);
		free(mt3);
		
	}
	else{
		printf("El numero de argumentos es incorrecto");
	}
	return 0;
}

#include <stdio.h>
#include <stdlib.h>


void llenar(int rows, int cols, int * mt[]){
	for (int i=0; i<rows; i++){
		for (int j=0; j< cols; j++){
			mt[i][j]= rand() % (20);  // de 0 a 20
		}			
	}
}

void imprimir(int rows, int cols, int * mt[]){
	for (int i=0; i<rows; i++){
		printf("\n");
		for (int j=0; j< cols; j++){
			printf("%i\t", mt[i][j]);
		}			
	}
	printf("\n");
}

void multiplicacion(int rows, int cols, int *mt1[], int *mt2[], int *mt3[]){
	for (int i=0; i<rows; i++){
		for (int j=0; j< cols; j++){
			mt3[i][j]=0;
			for(int k=0; k<cols; k++){
				mt3[i][j]+= mt1[i][k]*mt2[k][j];
			}
		}
	}
}

int main(int argc, char *argv[]){
	if (argc==3){
		int num_row= atoi(argv[1]);
		int num_col= atoi(argv[2]);

		int * mt1[num_row], *mt2[num_row], *mt3[num_row];

		for (int i=0; i<num_row; i++){
			mt1[i]= (int *)malloc(sizeof(int)*num_col);
			mt2[i]= (int *)malloc(sizeof(int)*num_col);
			mt3[i]= (int *)malloc(sizeof(int)*num_col);
		}
		llenar(num_row, num_col, mt1);
		llenar(num_row, num_col, mt2);
		imprimir(num_row, num_col, mt1);
		imprimir(num_row, num_col, mt2); 
		multiplicacion(num_row, num_col, mt1, mt2, mt3);
		imprimir(num_row, num_col, mt3);
		printf("fin");


	}
	else{
		printf("El numero de argumentos es incorrecto");
	}
	return 0;
}
#include <chrono>
#include <iostream>

int ** mt1, **mt2, **mt3;
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
	printf("\n");
}

//Realiza la multiplicacion de manera secuencial
void multiplicacion(void){
	for (int i=0; i<tam; i++){
		for (int j=0; j< tam; j++){
			mt3[i][j]=0;
			for(int k=0; k<tam; k++){
				mt3[i][j]+= mt1[i][k]*mt2[k][j];
			}
		}
	}
}

int main(int argc, char *argv[]){
	if (argc==2){

		tam= atoi(argv[1]);

		mt1= (int **)malloc(sizeof(int *)*tam);
		mt2= (int **)malloc(sizeof(int *)*tam);
		mt3= (int **)malloc(sizeof(int *)*tam);

		for (int i=0; i<tam; i++){
			mt1[i]= (int *)malloc(sizeof(int)*tam);
			mt2[i]= (int *)malloc(sizeof(int)*tam);
			mt3[i]= (int *)malloc(sizeof(int)*tam);
		}
		llenar(mt1);
		llenar(mt2);

		auto start = std::chrono::system_clock::now();
	    multiplicacion();
	    auto end = std::chrono::system_clock::now();

	    std::chrono::duration<float,std::ratio<1>> duration = end - start;

		printf("%d\t%.3f\n", atoi(argv[1]), duration.count());
		
		//imprimir(mt3);

		free(mt1);
		free(mt2);
		free(mt3);
		
	}
	else{
		printf("El numero de argumentos es incorrecto, solo ingrese el tamaño de la matriz");
	}
	return 0;
}
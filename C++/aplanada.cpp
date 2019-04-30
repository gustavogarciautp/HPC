#include <pthread.h>
#include <chrono>
#include <iostream>

using namespace std;

int  *mt1, *mt2, *mt3;
int tam;

void llenar(int * mt){
	for (int i=0; i< tam*tam; i++){
		mt[i]= 1+rand() % (20);
	}			
}

void imprimir(int *mt){
	for (int j=0; j< tam*tam; j++){
		if ((j%tam) == 0){
			printf("\n");
		}
		printf("%i\t", mt[j]);
	}
	printf("\n------------------------------------------------------------------------------------------------------------------\n");	
}
//Las matrices estan aplanadas por filas


//Cada hilo se encarga de calcular una unica fila
void * multiplicacion(void *threadid){

	long x= (long) threadid;;
	for (int j=x*tam; j< (x*tam+tam); j++){
		//int x= j/tam;
		for(int k= x*tam, l= j-(x*tam) ; k<(x*tam+tam); k++, l+=tam){
			mt3[j]+= mt1[k]*mt2[l];
		}
	}
	pthread_exit(NULL);
}

//Cada hila se encarga de calcular una unica columna
void * multiplicacion2(void *threadid){

	long x= (long) threadid;
	for (int j=0; j< tam*tam; ){
		for(int k= j, l= int(x); k<(j+tam); k++, l+=tam){
			mt3[j+x]+= mt1[k]*mt2[l];
		}
		j+=tam;
	}
	pthread_exit(NULL);
}


int main(int argc, char *argv[]){
	if (argc==2){
		tam= atoi(argv[1]);
		
		int size= tam*tam;

		mt1= (int *)malloc(sizeof(int)*size);
		mt2= (int *)malloc(sizeof(int)*size);
		mt3= (int *)malloc(sizeof(int)*size);
		
		llenar(mt1);
		llenar(mt2);
		
		pthread_t threads[tam];
		int rc, t;
		long taskids[tam];
		
		auto start = std::chrono::system_clock::now();

		for (t=0; t< tam; t++){
			taskids[t] = t;
			rc = pthread_create(&threads[t], NULL, multiplicacion2, (void *) taskids[t]);
			if (rc) {
			    //printf("ERROR; return code from pthread_create() is %d\n", rc);
			   exit(-1);
			}
		}
		for (int i=0; i< tam; i++){
			pthread_join (threads[i], NULL );
		}
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<float,std::ratio<1>> duration = end - start;

	    printf("%d\t%.3f\n", atoi(argv[1]), duration.count());
		//printf("Este es el codigo paralelo\n");
		//imprimir(mt1);
		//imprimir(mt2);
		//imprimir(mt3);
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
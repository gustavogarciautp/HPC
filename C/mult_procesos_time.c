#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int  **mt1, **mt2, **mt3;
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

void multiplicacion(int j){
	for (int i=0; i<tam; i++){
		//mt3[i][j]=0;
		for(int k=0; k<tam; k++){
			mt3[i][j]+= mt1[i][k]*mt2[k][j];
		}
		//printf("Proceso %i mt3[%i][%i]=%i\n", j, i,j, mt3[i][j]);
	}
}

int main(int argc, char *argv[]){
	if (argc==2){
		tam= atoi(argv[1]);  //Numero de filas, columnas y procesos (cada proceso calcula una columna)

		double total_time;
		clock_t start, end;

		//Se definen las variables compartidas
		mt1=(int **)mmap(NULL,sizeof(int *)*tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
		mt2=(int **)mmap(NULL,sizeof(int *)*tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
		mt3=(int **)mmap(NULL,sizeof(int *)*tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
		for (int i=0; i<tam; i++){
			mt1[i]=(int *)mmap(NULL,sizeof(int)*tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
			mt2[i]=(int *)mmap(NULL,sizeof(int)*tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
			mt3[i]=(int *)mmap(NULL,sizeof(int)*tam, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); //Reservar memoria para filas
		}

		llenar(mt1);
		llenar(mt2);
		
		int t, status[tam];
		pid_t pid_children[tam];

		start= clock();
		for (t=0; t<tam; t++) {

		  /*** error handling ***/
		  if ((pid_children[t] = fork()) < 0 ) {
		    printf ("No se pudo crear el subproceso\n");
		    exit(0);
		    }

		  /*** this is the child of the fork ***/
		  else if (pid_children[t] ==0) {
		    multiplicacion(t);
		    exit(0);
		    }
		  }
		 //Esperamos a que todos los procesos terminen
		 for (int h=0; h< tam; h++){
		 	waitpid(pid_children[h], &status[h], 0);
		 }
		end= clock();
		
		total_time= ((double) (end-start))/	CLOCKS_PER_SEC;
		printf("%d\t%.3f\n", atoi(argv[1]), total_time);
		//imprimir(mt1);
		//imprimir(mt2);
		//imprimir(mt3);
		//imprimir(mt3);
		//printf("Ultimo %i\n", mt3[tam-1][tam-1]);
		//Liberar memoria
		munmap(mt1, sizeof(int *)*tam);
		munmap(mt2, sizeof(int *)*tam);
		munmap(mt3, sizeof(int *)*tam);

	}
	else{
		printf("El numero de argumentos es incorrecto");
	}
}
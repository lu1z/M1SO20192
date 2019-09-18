//gcc -o singleVsMulti singleVsMulti.c -lpthread
//./singleVsMulti
//para alterar os valores das matrizes e seu tamanho 'e necess'ario
//modificar os valores em main()
//a matriz 'e abstraida em um vetor simples construido
//concatenando todas as linhas.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

struct data {
	int* a;
	int* b;
	int i;
    int col1;
    int j;
    int col2;
};

void* doSum(void *arg) {
	struct data *data_struct = (struct data*)arg;
	
	int sum = 0;
	for (int k = 0; k < data_struct->col1; k++)
		sum = sum + data_struct->a[data_struct->i * data_struct->col1 + k] * data_struct->b[k * data_struct->col2 + data_struct->j];
	
	int *answer = malloc(sizeof(*answer));
	*answer = sum;
	pthread_exit(answer);
}

void multiply(int *a, int row1, int col1, int *b, int row2, int col2, int *d) {
        for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            int sum = 0;
            for (int k = 0; k < col1; k++)
                sum = sum + a[i * col1 + k] * b[k * col2 + j];
            d[i * col2 + j] = sum;
        }
    }
}

void multiplyMulti(int *a, int row1, int col1, int *b, int row2, int col2, int *d) {
		pthread_t tids[row1 * col2];
		struct data args[row1 * col2];
		int* result;

        for (int i = 0; i < row1; i++) 
        for (int j = 0; j < col2; j++) {
			args[i * col2 + j].a = a;
			args[i * col2 + j].b = b;
			args[i * col2 + j].i = i;
			args[i * col2 + j].col1 = col1;
			args[i * col2 + j].j = j;
			args[i * col2 + j].col2 = col2;
            pthread_create(&tids[i * col2 + j], NULL, doSum, &args[i * col2 + j]);
        }
        
        for (int i = 0; i < row1; i++) 
        for (int j = 0; j < col2; j++) {
			
            pthread_join(tids[i * col2 + j], (void**)&result) ;
            
            *&d[i * col2 + j] = *result;
        }    
}

void print (int *d, int length, int colluns, double time_taken) {
    for (int i = 0; i < length; i++) {
        if (i % colluns == 0) {
            printf("\n");
        }
        printf("%d ", d[i]);
    }
    printf("\nmultiply() took %f seconds to execute \n", time_taken); 
}

void timedMultiplySingle(int *a, int row1, int col1, int *b, int row2, int col2, int *d) 
{

clock_t t;
t = clock();

multiply(a, row1, col1, b, row2, col2, d);

t = clock() - t; 
double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 


print(d, row1*col2, col2, time_taken);
}

void timedMultiplyMulti(int *a, int row1, int col1, int *b, int row2, int col2, int *d) 
{

clock_t t;
t = clock();

multiplyMulti(a, row1, col1, b, row2, col2, d);

t = clock() - t; 
double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 

print(d, row1*col2, col2, time_taken);
}


int main(void) {
    int row1 = 2;
    int col1 = 3;
    int row2 = 3;
    int col2 = 2;
    
    int m1[] = {1, 2, 3, 4, 5, 6};
    int m2[] = {7, 8, 9, 10, 11, 12};
    //int m1[row1 * col1];
    //int m2[row2 * col2];
    
    assert(col1 == row2);

//    int *d = malloc((row1*col2) * sizeof *d);

    int d[row1*col2];

    timedMultiplySingle(m1, row1, col1, m2, row2, col2, d);
    
    timedMultiplyMulti(m1, row1, col1, m2, row2, col2, d);
    
    return 0;
}

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>

#define NRA 62                
#define NCA 15                 
#define NCB 7                  

double calculateTimeDifference(struct timespec * endTime, struct timespec * beginTime)
{
	return ((double)endTime->tv_sec + 1.0e-9*endTime->tv_nsec) - ((double)beginTime->tv_sec + 1.0e-9*beginTime->tv_nsec);
}

int main (int argc, char *argv[]) 
{
	int	tid, nthreads, i, j, k, chunk;
	double	a[NRA][NCA],          
		b[NCA][NCB],           
		c[NRA][NCB];           
	struct timespec beginTime;
	struct timespec endTime;
	double timeDifference = 0.0;
	chunk = 10;               
	clock_gettime(CLOCK_MONOTONIC, &beginTime);
	#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
			printf("Uruchomiono na %d watkach\n",nthreads);
			printf("Inicjalizacja macierzy...\n");
		}

		#pragma omp for schedule (static, chunk) 
		for (i=0; i<NRA; i++)
		{
			for (j=0; j<NCA; j++)
			{
				a[i][j]= i+j;
			}
		}

		#pragma omp for schedule (static, chunk)
		for (i=0; i<NCA; i++)
		{
			for (j=0; j<NCB; j++)
			{
				b[i][j]= i*j;
			}
		}
	  	
		#pragma omp for schedule (static, chunk)
		for (i=0; i<NRA; i++)
		{
			for (j=0; j<NCB; j++)
			{
				c[i][j]= 0;
			}
		}
		
		printf("Watek %d rozpoczyna mnozenie\n",tid);
		#pragma omp for schedule (static, chunk)
		for (i=0; i<NRA; i++)    
		{
			printf("Watek=%d Wiersz=%d\n",tid,i);
			for(j=0; j<NCB; j++)
			{       
				for (k=0; k<NCA; k++)
				{
					c[i][j] += a[i][k] * b[k][j];
				}			
			}		
		}
	}  
	clock_gettime(CLOCK_MONOTONIC, &endTime);
	timeDifference = calculateTimeDifference(&endTime, &beginTime);
	printf("Macierz wynikowa:\n");
	for (i=0; i<NRA; i++)
	{
		for (j=0; j<NCB; j++) 
		{
			printf("%6.2f   ", c[i][j]);
		}
		printf("\n"); 
	}
	printf("Time difference: %f\n", timeDifference);
	return 0;
}


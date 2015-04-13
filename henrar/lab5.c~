#include <omp.h>
#include <stdio.h>

int main()
{
	int var1, var2, var3;
	#pragma omp parallel
	{
		#pragma omp for
		for(int i = 0; i < 10; i++)
		{
			printf("Operacje %d wykonuje: %d\n", i, omp_get_thread_num());
		}	
	}
	return 0;
}

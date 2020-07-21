#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<string.h>
#include<omp.h>
#define maxPrime 10000000000
#define maxstack 500000000
int top=-1;
int primestack[maxstack];
int total[16];
void push(int data)
{
	if(top>=maxstack)
	{
		printf("stack is full \n");
	}
	else
	{
		top++;
		primestack[top]=data;
	}
}
int main()
{
	int limit = floor(sqrt(maxPrime));
	long long i,j,k,lowlim;
	int tnum;
    double start,end;
	bool *mark=(bool*)malloc(limit+1);
	printf("number of thread:");
	scanf("%d",&tnum);
	omp_set_num_threads(tnum);
    start = omp_get_wtime(); 
    for ( i=2; i*i<limit; i++) 
    { 
        if (mark[i] == false) 
        { 
            for (int j=i*2; j<limit; j+=i) 
                mark[j] = true; 
        } 
    } 
    for ( i=2; i<limit; i++) 
    { 
        if (mark[i] == false) 
        { 
		  total[0]++;
          push(i);
        } 
    }
	printf("%d\n",top+1);
	//在maxPrime的根號內有幾個質數
	long long low = 0;
	long long high;
	#pragma omp parallel for private(i,k,j,high,lowlim)
	for(low=0;low<maxPrime;low=low+limit)
	{
		bool *vis=(bool*)malloc(limit+1);
		high = low+limit;
		if(high >=maxPrime)
			high = maxPrime;
		for(i=0;i<top+1;i++)
		{
			lowlim=floor(low/primestack[i]) * primestack[i];
			if(lowlim < low)
				lowlim+=primestack[i];
			for(j=lowlim;j<high;j+=primestack[i])
				vis[j-low] = true;
		}
		for(k=low;k<high;k++)
		{
			if(!vis[k-low])
			{
				//printf("%d from *%d*\n",k,omp_get_thread_num());
				total[omp_get_thread_num()]++;
			}
		}
	}
	for(i=1;i<16;i++)
	{
		total[0]+=total[i];
	}
	end = omp_get_wtime();
	printf("%d\n",total[0]-1);
	printf("花了 %f 秒\n",end-start);
	free(mark);

    return 0;
}
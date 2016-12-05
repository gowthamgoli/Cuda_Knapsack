#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>

#define index(i, j, N)  ((i)*(N+1)) + (j)

__device__ int maximum(int a, int b) { 
	return (a > b)? a : b; 
}

__global__ void knapsackKernel(int *profits, int *weights, int *f, int capacity, int i){

	int c = threadIdx.x;

	if(i==0 || c==0)	f[index(i,c,capacity)] = 0;
	else if(weights[i-1] <= c){
		f[index(i,c,capacity)] = maximum(f[index(i-1,c,capacity)], profits[i-1]+f[index(i-1,c-weights[i-1],capacity)]);
	}
	else
		f[index(i,c,capacity)] = f[index(i-1,c,capacity)];
}

void knapsackCuda(int *profits, int *weights, int c, int n, int *f){
	int *dev_profits, *dev_weights, *dev_f;

	cudaMalloc((void**)&dev_f, (n+1)*(c+1)*sizeof(int));
	cudaMalloc((void**)&dev_profits, n*sizeof(int));
	cudaMalloc((void**)&dev_weights, n*sizeof(int));

	cudaMemcpy(dev_profits, profits, n*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_weights, weights, n*sizeof(int), cudaMemcpyHostToDevice);

	int i=0;
	while(i<=n){
		knapsackKernel<<<1, c+1>>>(dev_profits, dev_weights, dev_f, c, i);
		i++;
	}

	cudaMemcpy(f, dev_f, (n+1)*(c+1)*sizeof(int), cudaMemcpyDeviceToHost);
	
	cudaFree(dev_profits);
	cudaFree(dev_weights);
	cudaFree(dev_f);
}

int main() {
	int i;
	int n = 3;
	int *profits = (int*)malloc(n*sizeof(int));
	int *weights = (int*)malloc(n*sizeof(int));

	FILE *myFile;
    myFile = fopen("rand.txt", "r");

	for (i = 0; i < n; i++)
    {
        fscanf(myFile, "%d %d", &profits[i], &weights[i]);
    }

    int capacity = 5;

    //int n = sizeof(profits)/sizeof(int);

    int *f = (int *)malloc((n+1)*(capacity+1)*sizeof(int));

    knapsackCuda(profits, weights, capacity, n, f);

    int c;
	for(i=0; i<=n ; i++){
		for(c=0; c<=capacity; c++){
			printf("%d ", f[index(i,c,capacity)]);
		}
		printf("\n");
	}

}
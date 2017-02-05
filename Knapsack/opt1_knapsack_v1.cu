#include <cuda.h>
#include <stdlib.h>
#include <stdio.h>

#define TILE_WIDTH 512
#define index(i, j, N)  ((i)*(N+1)) + (j)

int maximum(int a, int b) { 
	return (a > b)? a : b; 
}

__global__ void knapsackKernel(int *profits, int *weights, int *input_f, int *output_f, int capacity, int c_min, int k){

	int c = blockIdx.x*512 + threadIdx.x;
	if(c<c_min || c>capacity){return;}
	if(input_f[c] < input_f[c-weights[k-1]]+profits[k-1]){
		output_f[c] = input_f[c-weights[k-1]]+profits[k-1];
	}
	else{
		output_f[c] = input_f[c];
	}
}

void knapsackCuda(int *profits, int *weights, int capacity, int n, int *f0, int *f1){
	int *dev_profits, *dev_weights, *dev_f0, *dev_f1;
	int sumW = 0;
	int i,c; 

	for(i=0; i<n; i++){
		sumW = sumW + weights[i];
	}

	cudaMalloc((void**)&dev_f0, (capacity+1)*sizeof(int));
	cudaMalloc((void**)&dev_f1, (capacity+1)*sizeof(int));
	cudaMalloc((void**)&dev_profits, n*sizeof(int));
	cudaMalloc((void**)&dev_weights, n*sizeof(int));

	cudaMemcpy(dev_profits, profits, n*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_weights, weights, n*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemset(dev_f0, 0, (capacity+1)*sizeof(int));
	cudaMemset(dev_f1, 0, (capacity+1)*sizeof(int));

	/*int p;
	for(p=0; p<=capacity; p++)	printf("%d ", dev_f1[p]);
	printf("\n");*/

	int k=1;
	while(k<=n){
		sumW = sumW - weights[k-1];
		c = maximum(capacity-sumW, weights[k-1]);

		//printf("k = %d\n", k);
		//printf("%d\n", c);
		
		dim3 dimGrid(ceil(1.0*(capacity-0+1)/TILE_WIDTH), 1, 1);
		dim3 dimBlock(TILE_WIDTH,1,1);

		if(k%2==0){
			cudaMemcpy(dev_f1, dev_f0, (capacity+1)*sizeof(int), cudaMemcpyDeviceToDevice);
			knapsackKernel<<<dimGrid, dimBlock>>>(dev_profits, dev_weights, dev_f0, dev_f1, capacity, c, k);
			//cudaDeviceSynchronize();
			/*cudaMemcpy(f1, dev_f1, (capacity+1)*sizeof(int), cudaMemcpyDeviceToHost);
			int p;
			for(p=0; p<=capacity; p++)	printf("%d ", f1[p]);
			printf("\n");*/
		}
		else{
			cudaMemcpy(dev_f0, dev_f1, (capacity+1)*sizeof(int), cudaMemcpyDeviceToDevice);
			knapsackKernel<<<dimGrid, dimBlock>>>(dev_profits, dev_weights, dev_f1, dev_f0, capacity, c, k);	
			//cudaDeviceSynchronize();
			/*cudaMemcpy(f0, dev_f0, (capacity+1)*sizeof(int), cudaMemcpyDeviceToHost);
			int p;
			for(p=0; p<=capacity; p++)	printf("%d ", f0[p]);
			printf("\n");*/
		}
		k++;
	}

	cudaMemcpy(f0, dev_f0, (capacity+1)*sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(f1, dev_f1, (capacity+1)*sizeof(int), cudaMemcpyDeviceToHost);
	
	cudaFree(dev_profits);
	cudaFree(dev_weights);
	cudaFree(dev_f0);
	cudaFree(dev_f1);
}

int main() {
    int i;
	int n = 10000;
	int *profits = (int*)malloc(n*sizeof(int));
	int *weights = (int*)malloc(n*sizeof(int));

	FILE *myFile;
    myFile = fopen("rand.txt", "r");

	for (i = 0; i < n; i++)
    {
        fscanf(myFile, "%d %d", &profits[i], &weights[i]);
    }

    int capacity = 0;
	for(i=0; i<n; i++){
		capacity = capacity + weights[i];
	}
	capacity = capacity/2;
	//capacity = 1000;

	printf("capacity = %d\n", capacity);

    
    int *f0 = (int *)malloc((capacity+1)*sizeof(int));
    int *f1 = (int *)malloc((capacity+1)*sizeof(int));

    knapsackCuda(profits, weights, capacity, n, f0, f1);

    if(n%2==0){
    	//int p;
		//for(p=0; p<=capacity; p++){	printf("%d ", f1[p]);}
    	printf("%d\n", f1[capacity]);
    }
    else{
    	//int p;
		//for(p=0; p<=capacity; p++)	{printf("%d ", f0[p]);}
    	printf("%d\n", f0[capacity]);
    }
}

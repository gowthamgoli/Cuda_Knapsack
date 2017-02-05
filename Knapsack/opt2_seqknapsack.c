#include <stdio.h>
#include <stdlib.h>

#define index(i, j, N)  ((i)*(N+1)) + (j)

int max(int a, int b){
	return (a > b)? a : b;
}

void knapsack(int capacity, int profits[], int weights[], int n, int *f, int *M){
	int sumW = 0;
	int i,c,p; 

	for(i=0; i<n; i++){
		sumW = sumW + weights[i];
	}

	int k = 1;
	while(k<=n){
		//printf("k = %d\n", k);
		sumW = sumW - weights[k-1];
		c = max(capacity-sumW, weights[k-1]);
		//printf("%d %d\n", sumW, c);
		//printf("%d\n", c);
		for(p=capacity; p>=c; p--){
			if(f[p] < f[p-weights[k-1]] + profits[k-1]){
				f[p] = f[p-weights[k-1]] + profits[k-1];
				M[index(k,p,capacity)] = 1;
			}
		}
		/*int t;
		for(t=0; t<=capacity; t++)	printf("%d ", f[t]);
		printf("\n");*/	
		k++;
	}

	//for(i=0; i<=capacity; i++)
	printf("%d ", f[capacity]);
	printf("\n");
	int sum = 0;

	c = capacity;
	i = n;
	sum = 0;

	while(c>0 && i>0){
		while(M[index(i,c,capacity)] != 1){
			i = i-1;
		}
		//printf("%d ", i);
		sum = sum + profits[i-1];
		c = c-weights[i-1];
		i = i-1;	
	}
	printf("\n");

	/*for(i=1; i<=n; i++){
		if(M[i][capacity] == 1)	{
			printf("%d ", i);
			sum = sum + profits[i-1];
		}
	}*/
	printf("%d ", sum);
	/*for(i=0; i<=n; i++){
		for(c=0; c<= capacity; c++){
			printf("%d ", M[index(i,c,capacity)]);
		}
		printf("\n");
	}*/
}


int main(){

	int i,j;
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
	//capacity = capacity/2;
	capacity = 214000;
	printf("capacity = %d\n", capacity);

	int *f = (int*)malloc((capacity+1)*sizeof(int));
	int *M = (int*)calloc((n+1)*(capacity+1), sizeof(int*));
	

	for(i=0; i<=capacity; i++){
		f[i] = 0;
	}

	for(i=0; i<=n; i++){
		for(j=0; j<=capacity; j++){
			M[index(i,j,capacity)] = M[index(i,j,capacity)] + 1;
		}
	}
	
	//knapsack(capacity, profits, weights, n, f, M);

	free(f);
	free(M);

	return 0;

}

#include <stdio.h>
#include <stdlib.h>

int max(int a, int b){
	return (a > b)? a : b;
}

void knapsack(int capacity, int profits[], int weights[], int n, int *f){
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
		for(p=capacity; p>=c; p--){
			if(f[p] < f[p-weights[k-1]] + profits[k-1]){
				f[p] = f[p-weights[k-1]] + profits[k-1];
			}
		}
		/*int t;
		for(t=0; t<=capacity; t++)	printf("%d ", f[t]);
		printf("\n");*/
		k++;
	}

	for(i=0; i<=capacity; i++)
		printf("%d ", f[i]);
}


int main(){

	int i;
	int n = 1000;
	int *profits = (int*)malloc(n*sizeof(int));
	int *weights = (int*)malloc(n*sizeof(int));

	int capacity = 1000;

	int *f = (int*)malloc((capacity+1)*sizeof(int));

	for(i=0; i<=capacity; i++){
		f[i] = 0;
	}

	FILE *myFile;
    myFile = fopen("rand.txt", "r");

	for (i = 0; i < n; i++)
    {
        fscanf(myFile, "%d %d", &profits[i], &weights[i]);
    }
	
	knapsack(capacity, profits, weights, n, f);

}
#include <stdio.h>
#include <stdlib.h>

int max(int a, int b){
	return (a > b)? a : b;
}

void knapsack(int capacity, int profits[], int weights[], int n){
	
	int i,c;
	int f[n+1][capacity+1];

	for(i=0; i<=n; i++){
		for(c=0; c<=capacity; c++){
			if(i==0 || c==0)	f[i][c] = 0;
			else if(weights[i-1] <= c){
				f[i][c] = max(f[i-1][c], f[i-1][c-weights[i-1]]+profits[i-1]);
			}
			else	f[i][c] = f[i-1][c];
		}
	}

	/*for(i=0; i<=n ; i++){
		for(c=0; c<=capacity; c++){
			printf("%d ", f[i][c]);
		}
		printf("\n");
	}*/
	printf("%d\n", f[n][capacity]);
}

int main(){

	int i;
	int n = 1000;
	int *profits = (int*)malloc(n*sizeof(int));
	int *weights = (int*)malloc(n*sizeof(int));

	FILE *myFile;
    myFile = fopen("rand.txt", "r");

	for (i = 0; i < n; i++)
    {
        fscanf(myFile, "%d %d", &profits[i], &weights[i]);
    }
	int capacity = 1500;

	knapsack(capacity, profits, weights, n);

}

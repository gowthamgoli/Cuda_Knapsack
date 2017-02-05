#include <stdio.h>
#include <stdlib.h>

int max(int a, int b){
	return (a > b)? a : b;
}

void knapsack(int capacity, int profits[], int weights[], int n, int *f0, int *f1){
	
	int i,c;

	int *curr; 
	int *prev;
	
	for(i=1; i<=n; i++){
		if(i%2 == 0){
			curr = f1;
			prev = f0;
		}
		else{
			curr = f0;
			prev = f1;
		}	
		for(c=0; c<=capacity; c++){
			if(weights[i-1] <= c){
				curr[c] = max(prev[c], prev[c-weights[i-1]]+profits[i-1]);
			}
			else{
				curr[c] = prev[c];
			}
		}
	}

	if(n%2 == 0){
		for(c=0; c<=capacity; c++)	
			printf("%d ", f1[c]);
		printf("\n");
	}
	else{
		for(c=0; c<=capacity; c++)
			printf("%d ", f0[c]);
		printf("\n");
	}
}

int main(){

	int i;
	int n = 3;
	int *profits = (int*)malloc(n*sizeof(int));
	int *weights = (int*)malloc(n*sizeof(int));

	int capacity = 5;

	int *f0 = (int*)malloc((capacity+1)*sizeof(int));
	int *f1 = (int*)malloc((capacity+1)*sizeof(int));

	for(i=0; i<=capacity; i++){
		f0[i] = 0;
		f1[i] = 0;
	}

	FILE *myFile;
    myFile = fopen("rand.txt", "r");

	for (i = 0; i < n; i++)
    {
        fscanf(myFile, "%d %d", &profits[i], &weights[i]);
    }
	

	knapsack(capacity, profits, weights, n, f0, f1);

}

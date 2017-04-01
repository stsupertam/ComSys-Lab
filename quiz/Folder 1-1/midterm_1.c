#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sort(int* array, unsigned int length){
	int n,c,d,t;
	for(c=1; c < length ; c++){
		d = c;
		while(d>0 && array[d] < array[d-1]){
			t = array[d];
			array[d] = array[d-1];
			array[d-1] = t;
			d--;
		}
	}
	printf("Sorted list in ascending order:\n");
}

int main()
{
	FILE* file;
	unsigned int length;
	int* data;

	int n,c,d,t;
	file = fopen("data", "r");
  	fseek(file, 0L, SEEK_END);
  	length = ftell(file);
  	fseek(file, 0L, SEEK_SET);
	data = malloc(length * sizeof(int) + 1);
 	memset(data, 0, length);
	fread(data, sizeof(int), length, file);
	unsigned int i = 0;
	while(1){
		if(data[i] == 0) break;
		i++;
	}
	sort(data, i);

	for(c=0; c < i; c++){
		printf("%d ", data[c]);

	}
	return 0;
}

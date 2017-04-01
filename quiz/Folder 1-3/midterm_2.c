#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* file;
unsigned int length;
unsigned int array_length = 0;
int* array;

void sort(){
	int n,c,d,t;
	for(c=1; c < array_length ; c++){
		d = c;
		while(d>0 && array[d] < array[d-1]){
			t = array[d];
			array[d] = array[d-1];
			array[d-1] = t;
			d--;
		}
	}
	printf("Sorted list in ascending order:\n");	
	for(c=0; c < array_length; c++){
		printf("%d ", array[c]);
	}
}

void read(char *in){
	int n,c,d,t;
	file = fopen(in, "r");
  	fseek(file, 0L, SEEK_END);
  	length = ftell(file);
  	fseek(file, 0L, SEEK_SET);
	array = malloc(length * sizeof(int) + 1);
 	memset(array, 0, length);
	fread(array, sizeof(int), length, file);
	while(1){
		if(array[array_length] == 0) break;
		array_length++;
	}
}


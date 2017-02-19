#include<stdio.h>
int main(){
  int array[999], position, num, index=0;
  FILE *file;
  file = fopen("TEST.dat", "r");
  if (file) {
    while (fscanf(file, "%d", &num)!=EOF){
      array[index] = num;
      index++;
    }
    fclose(file);
  }
  for(int i=1; i<=array[0]; i++){
    position = i;
    for(int j=i+1; j<index; j++){
      if(array[position] > array[j])
        position = j;
    }
    if(position != i){
      int tmp = array[i];
      array[i] = array[position];
      array[position] = tmp;
    }
  }
  for(int i=1; i<=array[0]; i++){
    printf("%d\n", array[i]);
  }
}

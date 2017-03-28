#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

unsigned int string_search(char* data,unsigned int length, char* target) {
   unsigned int i;
   unsigned int found=0;
   printf("search %s\n",target);
   for (i=0;i < length-strlen(target); i++)
    {
       if (strncmp(&data[i],target,strlen(target))==0) {
       // printf("found...\n");
        found++;
       }
    }
   return found;
}


int main(int argc, char** argv) {
  FILE* file;
  unsigned int length;
  char* data;
  char target[256];
  unsigned int found;
  time_t a, b;

  /* get start time */
  a = time(NULL);

  /* open the file */
  if (argc < 2) {
    printf("Please pass an input file.\n");
    return 0;
  }
  file = fopen(argv[1], "r");
  if (!file) {
    printf("Could not open %s for reading.\n", argv[1]);
    return 0;
  }

  /* find the length of the file */
  fseek(file, 0L, SEEK_END);
  length = ftell(file);
  fseek(file, 0L, SEEK_SET);

  /* read the file into memory */
  data = malloc(length * sizeof(char) + 1);
  memset(data, 0, length);
  fread(data, sizeof(char), length, file);

  b = time(NULL);
  double read = difftime(b, a);
  printf("Reading took %lf seconds.\n", read);

  /* ask what should be searched for */
  printf("Enter search term: ");
  scanf("%s", target);

  a = time(NULL);
  /* now do the searching */
  found = string_search(data, length, target);
  b = time(NULL);
  double search = difftime(b, a);
  printf("Searching took %lf seconds.\n", search);


  if (found) {
    printf("Found it! %d times\n", found);
  } else {
    printf("Not found...\n");
  }

  return 0;
}


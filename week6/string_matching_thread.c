#include <pthread.h>
#include <stdio.h>

#define CHUNK 1000

int sharedByteRead = 0;
int sharedTotalCount = 0;

FILE *fp; // keep fp pointer for the text fp
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
int total_bytes =  0;// init  total bytes from fp.

    //void*  GetDataBytes(void* param)
    //{
    //   char data[CHUNK];
    //   int count;
    //    pthread_mutex_lock(&mutex);
    //    printf("Changing the shared resource now.\n");
    //     … Read the next 1000 bytes from fp using *fp …..
    //     … copy to its local array .. data ...
    //     ... if not more data ... exit() ...
    //     sharedByteRead += CHUNK;
    //
    //     pthread_mutex_unlock(&mutex);
    //     count=  Perform counting .....
    //
    //     pthread_mutex_ lock(&mutex2); // second mutex for summing count.
    //     sharedTotalCount += count;
    //     pthread_mutex_unlock(&mutex2);
    //    return 0;
    //}

void GetDataBytes(){
    char *data;
    int count;
    fseek(fp, 0L, SEEK_END);
    unsigned length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    data = malloc(CHUNK);
    memset(data, 0, length);
    fread(data, sizeof(char), 300, fp);
    printf("Data: %s\n", data);

}

int main(int argc, char **argv){
    if (argc < 2){
        printf("Please pass an input fp.\n");
        return 0;
    }
    fp = fopen(argv[1], "r");
    if (!fp){
        printf("Could not open %s for reading.\n", argv[1]);
        return 0;
    }
    GetDataBytes();

    //    pthread_t thread;
    //    void* ret = NULL;
    //    // Really not locking for any reason other than to make the point.
    //    ... start timer...
    //    pthread_create(&thread, NULL, fooAPI, NULL);   // create 1 thread
    //
    //
    //    pthread_t tid = something; /// change it suitably
    //    if (pthread_join (thread, &ret))  // wait for thread to finish
    //         print error ...
    //    else {
    //       report  sharedTotalCount ...
    //    }
    //   ... end timer...
}
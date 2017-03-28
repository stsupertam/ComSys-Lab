#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define CHUNK 1000

int sharedByteRead = 0;
int sharedTotalCount = 0;

FILE *fp; // keep fp pointer for the text fp
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

typedef struct str_thdata
{
    int thread_no;
} tname;

int string_search(char *data, int length, char *target)
{
    int i;
    int found = 0;
    for (i = 0; i < length - strlen(target); i++)
    {
        if (strncmp(&data[i], target, strlen(target)) == 0)
        {
            // printf("found...\n");
            found++;
        }
    }
    return found;
}

void *GetDataBytes(void *param)
{
    tname *name;
    name = (tname *) param;
    char data[CHUNK];
    int count;
    while (fread(data, sizeof(char), CHUNK, fp) == CHUNK)
    {

        //printf("Check:%s\n", data);
        //printf("Changing the shared resource now. Thread:%d\n", name->thread_no);
        pthread_mutex_lock(&mutex);
        sharedByteRead += CHUNK;
        pthread_mutex_unlock(&mutex);
    }

    count = string_search(data, CHUNK, "<http://www.w3.org/2001/XMLSchema#string>");

    pthread_mutex_lock(&mutex2); // second mutex for summing count.
    sharedTotalCount += count;
    pthread_mutex_unlock(&mutex2);
    pthread_exit(0);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Please pass an input fp.\n");
        return 0;
    }
    fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("Could not open %s for reading.\n", argv[1]);
        return 0;
    }
    fseek(fp, 0L, SEEK_END);
    unsigned total_bytes = ftell(fp) * 8;
    fseek(fp, 0L, SEEK_SET);
    pthread_t thread1, thread2, thread3, thread4;
    tname t1, t2, t3, t4;         /* structs to be passed to threads */
    t1.thread_no = 1;
    t2.thread_no = 2;
    t3.thread_no = 3;
    t4.thread_no = 4;
    void *ret = NULL;
    // Really not locking for any reason other than to make the point.
    //... start timer...
    pthread_create(&thread1, NULL, GetDataBytes, (void *) &t1);
    pthread_create(&thread2, NULL, GetDataBytes, (void *) &t2);
    pthread_create(&thread3, NULL, GetDataBytes, (void *) &t3);
    pthread_create(&thread4, NULL, GetDataBytes, (void *) &t4);
    pthread_join(thread1, &ret);
    pthread_join(thread2, &ret);
    pthread_join(thread3, &ret);
    pthread_join(thread4, &ret);

    printf("Total count: %d\n", sharedTotalCount);
}
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define CHUNK 1000

int sharedByteRead = 0;
int sharedTotalCount = 0;

FILE *fp; // keep fp pointer for the text fp
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
int total_bytes = 0; // init  total bytes from fp.

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
    printf("%d\n", found);
    return found;
}

void *GetDataBytes(void *param)
{
    char data[CHUNK];
    int count;
    printf("Changing the shared resource now.\n");
    while (!fread(data, sizeof(char), 125, fp))
    {
        printf("Test : %s\n", data);

        pthread_mutex_lock(&mutex);
        sharedByteRead += CHUNK;
        pthread_mutex_unlock(&mutex);

        count = string_search(data, CHUNK, "<http://www.w3.org/2001/XMLSchema#string>");

        pthread_mutex_lock(&mutex2); // second mutex for summing count.
        sharedTotalCount += count;
        pthread_mutex_unlock(&mutex2);
    }
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
    pthread_t t1, t2, t3, t4;
    void *ret = NULL;
    // Really not locking for any reason other than to make the point.
    //... start timer...
    pthread_create(&t1, NULL, GetDataBytes, NULL);
    pthread_create(&t2, NULL, GetDataBytes, NULL);
    pthread_create(&t3, NULL, GetDataBytes, NULL);
    pthread_create(&t4, NULL, GetDataBytes, NULL);
    pthread_join(t1, &ret);
    pthread_join(t2, &ret);
    pthread_join(t3, &ret);
    pthread_join(t4, &ret);
    //pthread_t tid = something;      /// change it suitably
    //if (pthread_join(thread, &ret)) // wait for thread to finish
    //    print error... else {
    //        report sharedTotalCount...}... end timer...
}
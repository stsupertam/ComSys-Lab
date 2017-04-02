#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define CHUNK 10000
#define NUM_THREADS 4

int sharedTotalCount = 0;
int target_length;
const char target[] = "<http://www.w3.org/2001/XMLSchema#string>";

FILE *fp; // keep fp pointer for the text fp
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *GetDataBytes(void *param)
{
    char data[CHUNK + 1];
    int i, len, count = 0, drop = 0;

    while (1)
    {
        pthread_mutex_lock(&mutex);
        len = fread(data, sizeof(char), CHUNK, fp);
        data[len] = '\0';
        i = len;
        for (i = len - 1; i >= 0; i--)
        {
            if (data[i] == ' ' || data[i] == '<' || data[i] == '\n')
            {
                break;
            }
            data[i] = '\0';
            fseek(fp, -1, SEEK_CUR);
        }
        fseek(fp, -1, SEEK_CUR);

        pthread_mutex_unlock(&mutex);

        int length = len - target_length;
        for (i = 0; i < length; i++)
        {
            if (strncmp(&data[i], target, strlen(target)) == 0)
            {
                count++;
            }
        }
        if (len != CHUNK)
            break;
    }

    pthread_mutex_unlock(&mutex);
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
    target_length = strlen(target);
    pthread_t threads[NUM_THREADS];
    printf("Search : <http://www.w3.org/2001/XMLSchema#string>\n");
    void *ret = NULL;
    time_t a, b;
    int i;
    a = time(NULL);
    for (i = 0; i < NUM_THREADS; i++)
    {
        int rc = pthread_create(&threads[i], NULL, GetDataBytes, NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            printf("ERROR\n");
        }
    }
    b = time(NULL);
    double search = difftime(b, a);
    printf("Searching took %lf seconds.\n", search);
    printf("Total count: %d\n", sharedTotalCount);
}
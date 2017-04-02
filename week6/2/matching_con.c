#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 500000
#define CONS_SIZE 20000
#define NUM_CONSUMERS 4

char buf[BUF_SIZE][CONS_SIZE];
int count[BUF_SIZE] = {};
const char target[] = "<http://www.w3.org/2001/XMLSchema#string>";
const char invalid[] = {' ','\n', '<'};
long long target_length;
FILE *fp; 
int sharedTotalCount=0;

typedef struct {
    size_t len; // number of items in the buffer
    pthread_mutex_t mutex; // needed to add/remove data from the buffer
    pthread_cond_t can_produce; // signaled when items are removed
    pthread_cond_t can_consume; // signaled when items are added
} buffer_t;

// produce random numbers
void* producer(void *arg) {
    buffer_t *buffer = (buffer_t*)arg;
    int len,i;
    while(1) {
        pthread_mutex_lock(&buffer->mutex);
        if(feof(fp)) {
            pthread_mutex_unlock(&buffer->mutex);
            for(i=0;i<NUM_CONSUMERS;i++) {
                pthread_mutex_lock(&buffer->mutex);
                pthread_cond_signal(&buffer->can_consume);
                pthread_mutex_unlock(&buffer->mutex);
            }
            return NULL;
        }
        if(buffer->len == BUF_SIZE) { // full
            // wait until some elements are consumed
            pthread_cond_wait(&buffer->can_produce, &buffer->mutex);
        }
        len = fread(buf[buffer->len], sizeof(char), CONS_SIZE, fp); 
        buf[buffer->len][len] = '\0';
        if(!feof(fp)) {
            while(!strchr(invalid, fgetc(fp))) {
                buf[buffer->len][--len] = '\0';
                fseek(fp, -2, SEEK_CUR);
            }
            fseek(fp,-1,SEEK_CUR);
        }
        ++buffer->len;

        // signal the fact that new items may be consumed
        pthread_cond_signal(&buffer->can_consume);
        pthread_mutex_unlock(&buffer->mutex);
    }
    pthread_exit(NULL);
}

// consume random numbers
void* consumer(void *arg) {
    buffer_t *buffer = (buffer_t*)arg;
    int len, i;
    while(1) {
        pthread_mutex_lock(&buffer->mutex);

        while(buffer->len == 0) { // empty
            // wait for new items to be appended to the buffer
            pthread_cond_wait(&buffer->can_consume, &buffer->mutex);
            if(feof(fp)) {
                pthread_cond_signal(&buffer->can_produce);
                pthread_mutex_unlock(&buffer->mutex);
                return NULL;
            }
        }

        --buffer->len;
        len = strlen(buf[buffer->len]) - target_length + 1;
        for(i=0;i<len;i++) {
            if(strncmp(&buf[buffer->len][i], target, target_length)==0) 
                ++count[buffer->len];
        }

        // signal the fact that new items may be produced
        pthread_cond_signal(&buffer->can_produce);
        pthread_mutex_unlock(&buffer->mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Error.\n");
        exit(-1);
    }
    fp = fopen(argv[1], "r");
    buffer_t buffer = {
        .len = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .can_produce = PTHREAD_COND_INITIALIZER,
        .can_consume = PTHREAD_COND_INITIALIZER
    };
    target_length = strlen(target);
    pthread_t prod, cons[NUM_CONSUMERS];
    time_t a, b;
    int i,sum = 0;
    printf("Searching : <http://www.w3.org/2001/XMLSchema#string>\n");
    a = time(NULL);

    pthread_create(&prod, NULL, producer, (void*)&buffer);
    for(i=0;i<NUM_CONSUMERS;i++) 
        pthread_create(&cons[i], NULL, consumer, (void*)&buffer);
    pthread_join(prod, NULL); // will wait forever
    for(i=0;i<NUM_CONSUMERS;i++) 
        pthread_join(cons[i], NULL);
    b = time(NULL);
    double search = difftime(b, a);
    printf("Searching took %lf seconds.\n", search);

    for(i=0;i<BUF_SIZE;i++)
        sum += count[i];

    printf("Total count: %d\n", sum);
    pthread_exit(NULL);
}
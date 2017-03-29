#define N  TOTALTHREAD
#define BUFF_SIZE 10000
#define CONSUME_SIZE 1000
int count[N];

producer()
{
     if buffer is full 
          then wait 
     put more content in BUFF_SIZE   
     when all words are gone, then go to wait for all consumers to finish.
     notify waiting consumer...that more data are produced.
}

consumer()
{
   if buffer is empty
     then wait()
   consume words (CONSUME_SIZE bytes)
     and count and save its array elements and put in count[tid]
   notify waiting  producer.
}


void main(){
 // create 1 producer
 // create N consumers.

 pthread_join .. wait for all consumers are done.
 sum all count values.

}
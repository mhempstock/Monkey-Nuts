/*Print all the combinations of nuts from 
1 100,000 for piping into excel */

#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <unistd.h>

int nuts;
pthread_mutex_t lock;
int counter;
int cpu_factor = 1;
int progress;



void *monCalc(void *vargp) 
{ 
    int* ul = (int*)vargp;
    int a,b,c;
    int tcount=0;
    int loop2=0;
    int loop3=0;
    // Loop over first part of the que
    for( a = ul[0]; a < ul[1]; a = a + 2 ){
        loop2 = (nuts - a) + 1;
        for( b = 1; b < loop2; b = b + 2 ){
            c = nuts - a - b;
            if (c > 0 && a!=b && a!=c && b!=c){
                tcount++;
            }
        }
    // report the progress of the outer
    pthread_mutex_lock(&lock);
    progress += 1;
    pthread_mutex_unlock(&lock);
   }
    pthread_mutex_lock(&lock);
    counter = counter + tcount;
    pthread_mutex_unlock(&lock);
    return NULL; 
} 


// Progress Counter
void *showProgress(void *vargp) 
{
    float nuts_to_cal;
    nuts_to_cal = nuts / 2;

    while(1)
    {
    sleep(1);
    pthread_mutex_lock(&lock);
    printf("\rCalculating number of combinations %.3f%% complete", (progress / nuts_to_cal) * 100 );
    fflush(stdout);
    pthread_mutex_unlock(&lock);
    }
    return NULL;
}
   
int main() 
{
    printf("How many nuts are there?\n");
    // scanf("%d",&nuts);
    for (nuts = 11; nuts <= 100001; nuts = nuts + 100) {
    // printf("Three monkeys ate a total of %d nuts.\n", nuts);
    // printf("Each monkey ate an odd number of nuts\n"); 
    // printf("and each monkeys eat a different amount of nuts\n");

    if(nuts % 2 == 0)
    {
        printf("for an even number of nuts there are 0 combinations");
        exit(0); 
    }
    // get number of processors
    long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN) * cpu_factor;
    int parts = (nuts / number_of_processors);
    int que[number_of_processors][2];
    pthread_t threads[number_of_processors];
    void * retvals[number_of_processors];
    int a = 0 ;
    int i = 0;
    int count = 0;
    double time_spent;
    clock_t begin, end;


    // Split the pool of nuts by the number of processors
    // and create a que
    begin = clock();
    que[0][1] = 1;

    // keep parts even
    if(parts % 2!= 0)
    {
        parts++;
    }
    for( a = 1; a <= number_of_processors; a++ ){
        que[a][1] = que[a - 1][1] + parts;
        que[a][0] = que[a - 1][1];
    }
    // fix the left over
    que[number_of_processors][1] = nuts;
    for( a = 1; a <= number_of_processors; a++ ){ 
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    // create the workers
    for (count = 1; count <= number_of_processors; ++count)
    {
      if (pthread_create(&threads[count], NULL, monCalc, (void*)(que[count])) != 0)
        {
          fprintf(stderr, "error: Cannot create thread # %d\n", count);
          break;
        }
    }

    // Start Progress counter
    // pthread_t p_thread;
    // pthread_create(&p_thread, NULL, showProgress, NULL); 

    // wait for workers to finish
    for (i = 1; i <= number_of_processors; ++i)
    {
      if (pthread_join(threads[i], &retvals[i]) != 0)
        {
          fprintf(stderr, "error: Cannot join thread # %d\n", i);
        }
    } 
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    pthread_mutex_destroy(&lock);
    printf("%d,%d\n",nuts, counter);
    counter = 0;
    progress = 0;
    // printf("\rThis gives a total %d combinations and some very fat monkeys\n",counter ); 
    // printf("CPU time to calculate %.3f\n", time_spent);
}   
    return 0;
}
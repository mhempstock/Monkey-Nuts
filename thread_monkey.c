/* First attempt to speed up calculations by splitting
the load over multiple threads.
*/

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
    int tcount,a,b,c;
    // Loop over first part of the que
    for( a = ul[0]; a < ul[1]; a = a + 2 ){
        for( b = 1; b < nuts; b = b + 2 ){
            for( c = 1; c < nuts; c = c + 2 ){
                if (a + b + c == nuts && a!=b && a!=c && b!=c){
                    tcount++;
                }
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
    // get number of processors
    long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN) * cpu_factor;
    int parts = (nuts / number_of_processors);
    int que[number_of_processors][2];
    pthread_t threads[number_of_processors];
    void * retvals[number_of_processors];
    int a;
    int i;
    int count;
    double time_spent;
    clock_t begin, end;

    // Get some input
    printf("How many nuts are there?\n");
    scanf("%d",&nuts);
    printf("Three monkeys ate a total of %d nuts.\n", nuts);
    printf("Each monkey ate an odd number of nuts\n"); 
    printf("and each monkeys eat a different amount of nuts\n");

    // We can't count even nuts so don't bother
    if(nuts % 2 == 0)
    {
        printf("for an even number of nuts there are 0 combinations");
        exit(0); 
    }


    begin = clock();
    // Split the pool of nuts by the number of processors
    // and create a que
    que[0][1] = 1;
    for( a = 1; a <= number_of_processors; a++ ){
        que[a][1] = que[a - 1][1] + parts;
        que[a][0] = que[a - 1][1];
    }
    // fix the left over
    que[number_of_processors][1] = nuts;


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
    pthread_t p_thread;
    pthread_create(&p_thread, NULL, showProgress, NULL); 

    // wait for workers to finish
    for (i = 1; i <= number_of_processors; ++i)
    {
      if (pthread_join(threads[i], &retvals[i]) != 0)
        {
          fprintf(stderr, "error: Cannot join thread # %d\n", i);
        }
    } 
    pthread_mutex_destroy(&lock);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\rThis gives a total %d combinations and some very fat monkeys\n",counter ); 
    printf("CPU time to calculate %.3f\n", time_spent);
    return 0;
}
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_key_t thread_log_key;

typedef struct
{   pthread_once_t once;
}pthread_str;

void close_thread_log(void *thread_log)
{   fclose((FILE *)thread_log);
}

void run(void)
{   printf("This subroutine is running at thread %d\n",(int)pthread_self());
    printf("'run' function is called only one times\n");

    pthread_key_create(&thread_log_key, close_thread_log);
}

void write_to_thread_log(const char* message)
{   FILE* thread_log = (FILE*)pthread_getspecific(thread_log_key);
    fprintf(thread_log, "%s\n",message);
}

static void* thread1(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_1. ID is %d\n",(int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 1 is starting!");
    return 0;
}
static void* thread2(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_2. ID is %d\n", (int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 2 is starting!");
    return 0;
}
static void* thread3(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_3. ID is %d\n", (int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 3 is starting!");
    return 0;
}
static void* thread4(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_4. ID is %d\n", (int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 4 is starting!");
    return 0;
}
static void* thread5(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_5. ID is %d\n", (int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 5 is starting!");
    return 0;
}

int main(void)
{   pthread_t   id[5];
    int i,ret;
    pthread_str instance;
    instance.once = PTHREAD_ONCE_INIT;

    ret = pthread_create(&(id[0]), NULL, thread1, &instance);
    ret = pthread_create(&(id[1]), NULL, thread2, &instance);
    ret = pthread_create(&(id[2]), NULL, thread3, &instance);
    ret = pthread_create(&(id[3]), NULL, thread4, &instance);
    ret = pthread_create(&(id[4]), NULL, thread5, &instance);

    printf("This is a main process\n");

    for(i=0; i<5; i++)
        pthread_join(id[i], NULL);

    return(0);
}

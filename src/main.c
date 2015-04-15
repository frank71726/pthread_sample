#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

pthread_key_t thread_log_key;

typedef struct
{
    pthread_once_t once;
    pthread_mutex_t lock;
    pthread_cond_t  cond_nonzero;
    int cond_count;
    int sum;
    int index;
}pthread_str;

void close_thread_log(void *thread_log)
{
    fclose((FILE *)thread_log);
}

void run(void)
{
    printf("This subroutine is running at thread %d\n",(int)pthread_self());
    printf("'run' function is called only one times\n");

    pthread_key_create(&thread_log_key, close_thread_log);
}

void write_to_thread_log(const char* message, pthread_str* ptr)
{
    int i;
    FILE* thread_log = (FILE*)pthread_getspecific(thread_log_key);
    fprintf(thread_log, "%s\n",message);

    switch(ptr->index)
    {
        case 1:
            for(i=1; i<=20; i++)
                ptr->sum += i;
        break;
        case 2:
            for(i=21; i<=40; i++)
                ptr->sum += i;
        break;
        case 3:
            for(i=41; i<=60; i++)
                ptr->sum += i;
        break;
        case 4:
            for(i=61; i<=80; i++)
                ptr->sum += i;
        break;
        case 5:
            for(i=81; i<=100; i++)
                ptr->sum += i;
        break;
        default:
        break;
    }
    ptr->index += 1;
    printf("At subroutine, sum = %d\n",ptr->sum);
}

static void* thread1(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_mutex_lock(&ptr->lock);

    while(ptr->cond_count != 1)
       pthread_cond_wait(&ptr->cond_nonzero, &ptr->lock);

    pthread_cond_signal(&ptr->cond_nonzero);
    ptr->cond_count ++; /* 2 */

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_1. ID is %d.\n",(int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 1 is starting!", ptr);

    pthread_mutex_unlock(&ptr->lock);
    pthread_exit(NULL);
    return 0;
}
static void* thread2(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_mutex_lock(&ptr->lock);

    while(ptr->cond_count != 2)
       pthread_cond_wait(&ptr->cond_nonzero, &ptr->lock);

    pthread_cond_signal(&ptr->cond_nonzero);
    ptr->cond_count ++; /* 3 */

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_2. ID is %d.\n",(int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 2 is starting!", ptr);

    pthread_mutex_unlock(&ptr->lock);
    pthread_exit(NULL);
    return 0;
}
static void* thread3(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_mutex_lock(&ptr->lock);

    while(ptr->cond_count != 3)
       pthread_cond_wait(&ptr->cond_nonzero, &ptr->lock);

    pthread_cond_signal(&ptr->cond_nonzero);
    ptr->cond_count ++; /* 4 */

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_3. ID is %d.\n",(int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 3 is starting!", ptr);

    pthread_mutex_unlock(&ptr->lock);
    pthread_exit(NULL);
    return 0;
}
static void* thread4(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_mutex_lock(&ptr->lock);

    while(ptr->cond_count != 4)
       pthread_cond_wait(&ptr->cond_nonzero, &ptr->lock);

    pthread_cond_signal(&ptr->cond_nonzero);
    ptr->cond_count ++;/* 5 */

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_4. ID is %d.\n",(int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 4 is starting!", ptr);

    pthread_mutex_unlock(&ptr->lock);
    pthread_exit(NULL);
    return 0;
}
static void* thread5(void *arg)
{
    FILE* thread_log;
    char thread_log_filename[50];
    pthread_str *ptr = (pthread_str*)arg;

    pthread_mutex_lock(&ptr->lock);

    while(ptr->cond_count != 5)
       pthread_cond_wait(&ptr->cond_nonzero, &ptr->lock);

    pthread_once((pthread_once_t *)&ptr->once,run);
    printf("this is pthread_5. ID is %d.\n",(int)pthread_self());
    sprintf(thread_log_filename, "thread%d.log",(int)pthread_self());
    thread_log = fopen(thread_log_filename, "w");
    pthread_setspecific(thread_log_key, thread_log);
    write_to_thread_log("Thread 5 is starting!", ptr);

    pthread_mutex_unlock(&ptr->lock);
    pthread_exit(NULL);
    return 0;
}

int main(void)
{
    pthread_t   id[5];
    int i;
    pthread_str instance;
    instance.once = PTHREAD_ONCE_INIT;
    instance.cond_count = 1;
    instance.sum = 0;
    instance.index = 1;

    if(pthread_mutex_init(&(instance.lock), NULL) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));
    if(pthread_cond_init(&(instance.cond_nonzero), NULL) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));
    if(pthread_create(&(id[0]), NULL, thread1, &instance) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));
    if(pthread_create(&(id[1]), NULL, thread2, &instance) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));
    if(pthread_create(&(id[2]), NULL, thread3, &instance) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));
    if(pthread_create(&(id[3]), NULL, thread4, &instance) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));
    if(pthread_create(&(id[4]), NULL, thread5, &instance) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));


    pthread_mutex_lock(&(instance.lock));
    printf("------This is a main process------\n");
    pthread_mutex_unlock(&(instance.lock));

    for(i=0; i<5; i++)
        pthread_join(id[i], NULL);

    printf("\nAt main, sum = %d\n",instance.sum);
    printf("At main, cond_count = %d\n",instance.cond_count);

    if(pthread_cond_destroy(&instance.cond_nonzero) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));
    return(0);
}

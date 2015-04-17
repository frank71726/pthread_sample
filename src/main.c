#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#define TOTAL_DATA  5
#define THREAD_NUM  8
pthread_key_t thread_log_key;

typedef struct node
{
    int num;
    char* text;
    struct node* next;
}NODE;

typedef struct
{
    pthread_once_t once;
    pthread_mutex_t lock;
    pthread_cond_t  cond_nonzero;
    sem_t sema_flag;
    int cond_count;
    int sum;
    int index;
    NODE* llnode_head;
}pthread_str;

int length_node(NODE* ptr)
{/* calculate length from ptr node*/
    int num = 1;
    NODE* next = ptr->next;

    while(next != NULL)
    {
        num++;
        next = next->next;
    }
    return(num);
}
NODE* getnode(void)
{
    NODE* node;

    node = (NODE*)malloc(sizeof(NODE));
    if(node == NULL)
        printf("memory is not enough!\n");
    return(node);
}

NODE* gotonextnode(NODE* now_node)
{
    if(now_node->next == NULL)/* get to last node*/
        return NULL;
    else
        return(now_node->next);/* next node */
}
NODE* gotolastnode(NODE* head)
{/* move to last node.*/
    NODE* node;

    node = head;
    while( node->next!= NULL)
        node= node->next;
    return(node);
}

void freenode(NODE* ptr)
{
    free(ptr->text);
    free(ptr);
}

NODE* delete_node(NODE* head, NODE* ptr)
{
    NODE* previous;

    if(ptr == head)/* delete first node*/
    {
        head = head->next;
    }else
    {
        previous = head;
        while(previous->next != ptr)/* find node in front of ptr*/
            previous = previous->next;

        if(ptr->next == NULL)/* ptr is last node*/
            previous->next = NULL;
        else
            previous->next = ptr->next;/* ptr is bot last node*/
    }
    freenode(ptr);
    return(head);
}

void delete_llist(NODE* head)
{
    while((head = delete_node(head, head)) != NULL);
}
NODE* find_node(NODE* head, int num)
{/* find nm from head*/
    NODE* ptr;

    ptr = head;/* start from head node*/
    while(ptr != NULL)
    {
        if(ptr->num == num)/* num match */
            return(ptr);
        else
            ptr = ptr->next;/* not math, move to next node.*/
    }
    return(ptr);
}
NODE* search_node(NODE* p, int num)
{/* search from any node*/
    NODE* node;

    node = p->next;/* point to next node*/
    while((node != NULL) && (node->num != num))
        node = node->next;/* move to next node*/

    return(node);
}

NODE* insert_node(NODE* head, NODE* ptr, int value, char* str)
{
    NODE* new_node;

    new_node = getnode();
    new_node->num = value;
    new_node->text= (char*)malloc(sizeof(char)*strlen(str));
    strcpy(new_node->text, str);
    new_node->next = NULL;

    if(ptr == NULL)/*insert at first node*/
    {
        new_node->next = head;
        head = new_node;
    }else
    {
        if(ptr->next == NULL)/* inset at last node*/
            ptr->next = new_node;/* point to last node*/
        else
        {/* inset to node except for first and last node.*/
           new_node->next = ptr->next;/* point to next node*/
           ptr->next = new_node;/* change to new node*/
        }
    }
    return(head);
}

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

static void* sem_handler1(void* arg)
{
    NODE* insert_po;
    char name1[] = "Thsi sem_handler1, 1111111!";
    char name2[] = "Thsi sem_handler1, 2222222!";
    pthread_str *ptr = (pthread_str*)arg;
    printf("At sem_handler1. \n");
    /*    ptr->sem_string[0] = (char*)malloc(sizeof(char) * strlen("This is sem_handler1."));*/
    /*ptr->sem_string[0] = strdup("This is sem_handler1.");*/

    insert_po = gotolastnode(ptr->llnode_head);
    ptr->llnode_head = insert_node(ptr->llnode_head, insert_po, 1, name1);
    insert_po = gotolastnode(ptr->llnode_head);
    ptr->llnode_head = insert_node(ptr->llnode_head, insert_po, 2, name2);

    sem_post(&ptr->sema_flag);

    pthread_exit(NULL);
    return 0;
}

static void* sem_handler2(void* arg)
{
    NODE* insert_po;
    char name3[] = "Thsi sem_handler1, 3333333!";
    char name4[] = "Thsi sem_handler1, 4444444!";
    pthread_str *ptr = (pthread_str*)arg;
    printf("At sem_handler2. \n");
    /*  ptr->sem_string[1] = (char*)malloc(sizeof(char) * strlen("This is sem_handler2."));*/
    /*ptr->sem_string[1] = strdup("This is sem_handler2.");*/

    insert_po = gotolastnode(ptr->llnode_head);
    ptr->llnode_head = insert_node(ptr->llnode_head, insert_po, 3, name3);
    insert_po = gotolastnode(ptr->llnode_head);
    ptr->llnode_head = insert_node(ptr->llnode_head, insert_po, 4, name4);

    sem_post(&ptr->sema_flag);

    pthread_exit(NULL);
    return 0;
}
static void* sem_handler3(void* arg)
{/* print all data*/
    static int num=0;
    NODE* now_position;
    pthread_str *ptr = (pthread_str*)arg;
    printf("At sem_handler3. \n");

    while(num != TOTAL_DATA)/* it should be 5 records. */
    {
        sem_wait(&ptr->sema_flag);
        num=0;
        now_position = ptr->llnode_head;
        printf("Currently included %d data.\n",length_node(ptr->llnode_head));

        do
        {
            printf("======> num=%d, test=%s\n",now_position->num, now_position->text);
            num += 1;
            now_position = gotonextnode(now_position);
        }while(now_position != NULL);
    }

    pthread_exit(NULL);
    return 0;
}
int main(void)
{
    NODE* test_node;
    char str_main[] = "This in main function!";
    pthread_t   id[THREAD_NUM];
    int i;
    pthread_str instance;
    instance.once = PTHREAD_ONCE_INIT;
    instance.cond_count = 1;
    instance.sum = 0;
    instance.index = 1;
    instance.llnode_head = getnode();

    /* setting first linklist value*/
    instance.llnode_head->num = 0;
    instance.llnode_head->text = (char*)malloc(sizeof(char)*strlen(str_main));
    strcpy(instance.llnode_head->text, str_main);
    instance.llnode_head->next = NULL;

    if(sem_init(&instance.sema_flag, 0, 0) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_mutex_init(&(instance.lock), NULL) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_cond_init(&(instance.cond_nonzero), NULL) != 0)
        printf(" The following error occured %s ", strerror(errno));
    /* for mutex */
    if(pthread_create(&(id[0]), NULL, thread1, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_create(&(id[1]), NULL, thread2, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_create(&(id[2]), NULL, thread3, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_create(&(id[3]), NULL, thread4, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_create(&(id[4]), NULL, thread5, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));
    /* for semaphore */
    if(pthread_create(&(id[5]), NULL, sem_handler1, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_create(&(id[6]), NULL, sem_handler2, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));
    if(pthread_create(&(id[7]), NULL, sem_handler3, &instance) != 0)
        printf("The following error occured %s ", strerror(errno));

    pthread_mutex_lock(&(instance.lock));
    printf("------This is a main process------\n");
    pthread_mutex_unlock(&(instance.lock));

    for(i=0; i<THREAD_NUM; i++)
        pthread_join(id[i], NULL);

    sem_destroy(&instance.sema_flag);

    printf("\nAt main, sum = %d\n",instance.sum);
    printf("At main, cond_count = %d\n",instance.cond_count);

    if(pthread_cond_destroy(&instance.cond_nonzero) != 0)
        printf("Oh dear, something went wrong with strerror(errno)! ===> %s ", strerror(errno));

    /**************************************************************************/
    /* for test search_nodeand find_node functions*/
    test_node = search_node(instance.llnode_head, 3);
    if(test_node != NULL)
        printf("XXX. num=%d, text=%s\n",test_node->num, test_node->text);
    else
        printf("search node fila\n");

    test_node = find_node(instance.llnode_head, 1);
    if(test_node != NULL)
        printf("XXX. num=%d, text=%s\n",test_node->num, test_node->text);
    else
        printf("find node fila\n");

/*    int ch;
    printf("press enter"); fflush(stdout);
    while(((ch = getchar()) != '\n') && (ch != EOF));*/
    /**************************************************************************/

    delete_llist(instance.llnode_head);
    printf("delete_llist successfully!\n");
    return(0);
}

#ifndef __LIST_H__
#define __LIST_H__

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

    int   (*ll_length_node)(NODE* ptr);
    NODE* (*ll_getnode)(void);
    NODE* (*ll_gotonextnode)(NODE* now_node);
    NODE* (*ll_gotolastnode)(NODE* head);
    void  (*ll_freenode)(NODE* ptr);
    NODE* (*ll_delete_node)(NODE* head, NODE* ptr);
    void  (*ll_delete_llist)(NODE* head);
    NODE* (*ll_find_node)(NODE* head, int num);
    NODE* (*ll_search_node)(NODE* p, int num);
    NODE* (*ll_insert_node)(NODE* head, NODE* ptr, int value, char* str);

}pthread_str;

void SysLLFuncInit(pthread_str *arg);

pthread_key_t thread_log_key;
extern pthread_str instance;
#endif

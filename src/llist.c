#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#include "llist.h"

pthread_str instance;

static int length_node(NODE* ptr)
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
static NODE* getnode(void)
{
    NODE* node;

    node = (NODE*)malloc(sizeof(NODE));
    if(node == NULL)
        printf("memory is not enough!\n");
    return(node);
}

static NODE* gotonextnode(NODE* now_node)
{
    if(now_node->next == NULL)/* get to last node*/
        return NULL;
    else
        return(now_node->next);/* next node */
}
static NODE* gotolastnode(NODE* head)
{/* move to last node.*/
    NODE* node;

    node = head;
    while( node->next!= NULL)
        node= node->next;
    return(node);
}

static void freenode(NODE* ptr)
{
    free(ptr->text);
    free(ptr);
}

static NODE* delete_node(NODE* head, NODE* ptr)
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

static void delete_llist(NODE* head)
{
    while((head = delete_node(head, head)) != NULL);
}
static NODE* find_node(NODE* head, int num)
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
static NODE* search_node(NODE* p, int num)
{/* search from any node*/
    NODE* node;

    node = p->next;/* point to next node*/
    while((node != NULL) && (node->num != num))
        node = node->next;/* move to next node*/

    return(node);
}

static NODE* insert_node(NODE* head, NODE* ptr, int value, char* str)
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

void SysLLFuncInit(pthread_str *arg)
{
    arg->ll_length_node = length_node;
    arg->ll_getnode = getnode;
    arg->ll_gotonextnode = gotonextnode;
    arg->ll_gotolastnode = gotolastnode;
    arg->ll_freenode = freenode;
    arg->ll_delete_node = delete_node;
    arg->ll_delete_llist = delete_llist;
    arg->ll_find_node = find_node;
    arg->ll_search_node = search_node;
    arg->ll_insert_node = insert_node;
    /* get head node*/
    arg->llnode_head = getnode();
}


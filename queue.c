#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */
/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new = malloc(sizeof(struct list_head));
    if (!new)
        return NULL;

    INIT_LIST_HEAD(new);
    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *l) 
{
    if (!l)
        return;
    
    element_t *node, *next;
    list_for_each_entry_safe(node, next, l, list){
        list_del(&node->list);
        q_release_element(node);
    }

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;

    INIT_LIST_HEAD(&new->list);
    int str_size = strlen(s);
    new->value = malloc((str_size + 1) * sizeof(char));

    if (!new->value) {
        free(new);
        return false;
    }

    strncpy(new->value, s, str_size);
    *(new->value + str_size) = '\0';
    list_add(&new->list, head);
    
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new = malloc(sizeof(element_t));
    if(!new)
        return false;
    
    INIT_LIST_HEAD(&new->list);
    int str_size = strlen(s);
    new->value = malloc((str_size + 1) * sizeof(char));

    if(!new->value){
        free(new);
        return false;
    }

    strncpy(new->value, s, str_size);
    *(new->value + str_size) = '\0';
    list_add_tail(&new->list, head);
    
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *target = list_entry(head->next, element_t, list);
    list_del(&target->list);

    if (bufsize){
        strncpy(sp, target->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    
    return target;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *target = list_entry(head->prev, element_t, list);
    list_del(&target->list);

    if(bufsize){
        strncpy(sp, target->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }

    return target;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    
    int size = 0;
    struct list_head *node;
    list_for_each (node, head)
        size++;

    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || !head->next)
        return false;

    struct list_head *rabbit = head->next, *turtle = head->next;

    while (rabbit != head && rabbit->next != head){
        rabbit = rabbit->next->next;
        turtle = turtle->next;
    }

    list_del(turtle);
    q_release_element(list_entry(turtle, element_t, list));
    
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

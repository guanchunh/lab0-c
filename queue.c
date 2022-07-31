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
    if (!head || list_empty(head) || list_is_singular(head))
        return false;
    
    struct list_head *curr = head->next, *next = curr->next;
    bool key = false;

    while (curr != head && next != head){
        element_t *curr_entry = list_entry(curr, element_t, list);
        element_t *next_entry = list_entry(next, element_t, list);

        while(next != head && !strcmp(curr_entry->value, next_entry->value)){
            list_del(next);
            q_release_element(next_entry);
            next = curr->next;
            next_entry = list_entry(next, element_t, list);
            key = true;
        }

        if (key){
            list_del(curr);
            q_release_element(curr_entry);
            key = false;
        }

        curr = next;
        next = next->next;
    }
    
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;

    struct list_head *first = head->next;

    while (first != head && first->next != head){
        struct list_head *second = first->next;
        list_del_init(first);
        list_add(first, second);
        first = first->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) 
{
    if (!head)
        return;
    
    struct list_head *prev = head->prev, *curr = head, *next = NULL;

    while (next != head){

        next = curr->next;
        curr->next = prev;
        curr->prev = next;
        prev = curr;
        curr = next;
    }
    
    
}

static struct list_head *mergelist(struct list_head *list1, struct list_head *list2)
{
    struct list_head *res = NULL;
    struct list_head **indirect = &res;
    for (struct list_head **node = NULL; list1 && list2; *node = (*node)->next)
    {
        element_t *list1_entry = list_entry(list1, element_t, list);
        element_t *lisr2_entry = list_entry(list2, element_t, list);
        node = strcmp(list1_entry->value, list2_entry->value) < 0 ? &list1 : &list2;

        *indirect = *node;
        indirect = &(*indirect)->next; 
    }
    *indirect = (struct list_head *) ((u_int64_t) list1 | (u_int64_t) list2);
    return res;
}

static struct list_head *mergesort(struct list_head *head)
{
    if (!head || !head->next)
        return head;

    struct list_head *rabbit = head, *turtle = head;
    while (rabbit && rabbit->next){
        rabbit = rabbit->next->next;
        turtle = turtle->next;
    } 

    struct list_head *mid = turtle;
    turtle->prev->next = NULL;

    struct list_head *left = mergesort(head);
    struct list_head *right = mergesort(mid);
    
    return mergelist(left,right); 
    
}
/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) 
{
    if (!head || list_empty(head))
        return ;

    head->prev->next = NULL;
    head->next = mergesort(head->next);

    struct list_head *curr = head, *next = head->next;

    while (next){
        next->prev = curr;
        curr = next;
        next = next->next;
    }

    curr->next = head;
    head->prev = curr;
    
}

//
// Created by 杨海 on 2020/6/10.
//

#include "list.h"
#include "stdlib.h"

//创建链表
list *listCreate(void) {
    struct list *list;
    list = malloc(sizeof(*list));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;
    list->lens = 0;
    return list;
}

//释放整个链表
void listRelease(list *list) {
    unsigned long lens;
    listNode *current;
    listNode *next;
    lens = list->lens;
    current = list->head;
    while (lens) {
        next = current->next;
        if (list->free) {
            //释放当前value的指针
            list->free(current->value);
        }
        //释放当前节点
        free(current);
        current = next;
        lens--;
    }
    //释放整个链表
    free(list);
}

//从头部添加节点
list *listAddNodeHead(list *list, void *value) {
    listNode *newNode;
    newNode = malloc(sizeof(*newNode));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->value = value;
    if (list->lens == 0) {
        //没有任何元素时
        list->head = newNode;
        list->tail = newNode;
        newNode->next = NULL;
        newNode->pre = NULL;
    } else {
        newNode->pre = NULL;
        newNode->next = LIST_FIRST(list);
        list->head->pre = newNode;
        list->head = newNode;
    }
    list->lens++;
    return list;
}

//从尾部添加节点
list *listAddNodeTail(list *list, void *value) {
    listNode *newNode;
    unsigned long lens = list->lens;
    newNode = malloc(sizeof(*newNode));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->value = value;
    if (lens == 0) {
        list->head = newNode;
        list->tail = newNode;
        newNode->pre = NULL;
        newNode->next = NULL;
    } else {
        newNode->next = NULL;
        newNode->pre = LIST_TAIL(list);
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->lens++;
    return list;
}

//创建一个包含值 value 的新节点，并将它插入到 old_node 的之前或之后
list *listInsertNode(list *list, listNode *old_node, void *value, int after) {
    unsigned long lens = list->lens;
    listNode *newNode;
    newNode = malloc(sizeof(*newNode));
    if (NULL == newNode) {
        return NULL;
    }
    newNode->value = value;
    if (after > 0) {
        newNode->next = old_node;
        if (NULL == old_node->pre) {
            //可能是头节点
            newNode->next = old_node;
            newNode->pre = NULL;
            list->head = newNode;
        } else {
            newNode->pre = old_node->pre;
            old_node->pre->next = newNode;
            old_node->pre = newNode;
        }
    } else {
        old_node->next = newNode;
        if (NULL == old_node->next) {
            //尾部节点
            newNode->pre = old_node;
            newNode->next = NULL;
            list->tail = newNode;
        } else {
            newNode->pre = old_node;
            newNode->next = old_node->next;
            old_node->next->pre = newNode;
        }
    }
    list->lens++;
    return list;
}

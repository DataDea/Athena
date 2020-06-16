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
//    if (after > 0) {
//        newNode->next = old_node;
//        if (NULL == old_node->pre) {
//            //可能是头节点
//            newNode->next = old_node;
//            newNode->pre = NULL;
//            list->head = newNode;
//        } else {
//            newNode->pre = old_node->pre;
//            old_node->pre->next = newNode;
//            old_node->pre = newNode;
//        }
//    } else {
//        old_node->next = newNode;
//        if (NULL == old_node->next) {
//            //尾部节点
//            newNode->pre = old_node;
//            newNode->next = NULL;
//            list->tail = newNode;
//        } else {
//            newNode->pre = old_node;
//            newNode->next = old_node->next;
//            old_node->next->pre = newNode;
//        }
//    }
    if (after) {
        //after不等于零表示插在节点的后面
        newNode->pre = old_node;
        newNode->next = old_node->next;
        if (list->tail == old_node) {
            list->tail = newNode;
        }
    } else {
        newNode->next = old_node;
        newNode->pre = old_node->pre;
        if (list->head == old_node) {
            list->head = newNode;
        }
    }
    //更新相邻节点的前后指针
    if (newNode->pre != NULL) {
        newNode->pre->next = newNode;
    }
    if (newNode->next != NULL) {
        newNode->next->pre = newNode;
    }
    list->lens++;
    return list;
}

//删除指定的节点
void listDelNode(list *list, listNode *node) {
    unsigned long lens = list->lens;
    if (lens == 0) {
        return;
    }
    if (node->pre != NULL) {
        node->pre->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next != NULL) {
        node->next->pre = node->pre;
    } else {
        list->tail = node;
    }

    if (list->free) {
        list->free(node->value);
    }
    free(node);
    list->lens--;
}

//创建一个迭代器
list_iterator *listGetIterator(list *list, int direction) {
    unsigned long lens;
    lens = list->lens;
    if (lens == 0) {
        return NULL;
    }
    list_iterator *iterator;
    if ((iterator = malloc(sizeof(*iterator))) == NULL) {
        return NULL;
    }
    if (direction) {
        //从头
        iterator->direction = LIST_START_HEAD;
        iterator->next = list->head;
    } else {
        //从尾
        iterator->direction = LIST_START_TAIL;
        iterator->next = list->tail;
    }
    return iterator;
}

//迭代器的下一个元素
listNode *listNext(list_iterator *iter) {
    listNode *current = iter->next;
    if (current != NULL) {
        if (iter->direction) {
            iter->next = current->next;
        } else {
            iter->next = current->pre;
        }
    }
    return current;
}

//释放迭代器
void listReleaseIterator(list_iterator *iter) {
    free(iter);
}

//复制整个链表
list *listDup(list *orig) {

    list *copy;
    list_iterator *iterator;
    listNode *current;

    if ((copy = listCreate()) == NULL) {
        return NULL;
    }
    copy->free = orig->free;
    copy->match = orig->match;
    copy->dup = orig->dup;

    //创建迭代器
    if ((iterator = listGetIterator(orig, LIST_START_HEAD)) == NULL) {
        return NULL;
    }

    //迭代器遍历原始的链表添加到新的链表里
    while ((current = listNext(iterator)) != NULL) {
        void *value;
        if (copy->dup) {
            if ((value = copy->dup(current->value)) == NULL) {
                //释放内存
                listRelease(copy);
                listReleaseIterator(iterator);
                return NULL;
            }
        } else {
            value = current->value;
        }
        if (listAddNodeTail(copy, value) == NULL) {
            listRelease(copy);
            listReleaseIterator(iterator);
            return NULL;
        }
    }
    //释放内存
    free(iterator);
    //返回副本
    return copy;
}

//节点查找功能
listNode *listSearchKey(list *list, void *key) {

    list_iterator *iterator;
    listNode *find = NULL;
    if ((iterator = listGetIterator(list, LIST_START_HEAD)) == NULL) {
        return NULL;
    }
    while ((find = listNext(iterator)) != NULL) {
        if (list->match != NULL) {
            if (list->match(find->value, key)) {
                //释放内存
                listReleaseIterator(iterator);
                return find;
            }
        } else {
            if (find->value == key) {
                listReleaseIterator(iterator);
                return find;
            }
        }
    }
    //及时没找到，也要释放内存
    listReleaseIterator(iterator);
    return find;
}

//去对应索引位置的节点
listNode *listIndex(list *list, long index) {
    listNode *find;

    //index小于零时从表尾开始找
    if (index < 0) {
        index = (-index) - 1;
        find = LIST_TAIL(list);
        while (index-- && find != NULL) find = find->pre;
    } else {
        //从表头开始中
        find = LIST_FIRST(list);
        while (index-- && find != NULL) find = find->next;
    }
    return find;
}

//将迭代器的方向设置为 LIST_START_HEAD 并将迭代指针重新指向表头节点
void listRewindHead(list *list, list_iterator *iterator) {
    iterator->next = list->head;
    iterator->direction = LIST_START_HEAD;
}

//将迭代器的方向设置为 LIST_START_TAIL 并将迭代指针重新指向表头节点
void listRewindTail(list *list, list_iterator *iterator) {
    iterator->next = list->tail;
    iterator->direction = LIST_START_TAIL;
}

//取出链表的表尾节点，并将它移动到表头，成为新的表头节点
void listRotate(list *list) {
    unsigned long lens = list->lens;
    if (lens <= 1) {
        return;
    }
    //取出表未几点
    listNode *last = list->tail;
    list->tail = last->pre;
    last->pre->next = NULL;

    //插入到表头
    last->next = list->head;
    last->pre = NULL;
    list->head->pre = last;
    list->head = last;
}


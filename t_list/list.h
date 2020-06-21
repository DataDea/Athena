//
// Created by 杨海 on 2020/6/9.
//

//redis 双端链表数据结构
#ifndef ATHENA_LIST_H
#define ATHENA_LIST_H

//常用的宏函数
#define LIST_START_HEAD 1
#define LIST_START_TAIL 0

#define LIST_LENGTH(l) ((l)->lens)
#define LIST_FIRST(l) ((l)->head)
#define LIST_TAIL(l) ((l)->tail)
#define LIST_NEXT(n) ((n)->next)
#define LIST_PRE(n) ((n)->pre)
#define LIST_VALUE(n) ((n)->value)
#define LIST_RESET_DUMP_METHOD(l, m) ((l)->dup = (m))
#define LIST_RESET_FREE_METHOD(l, m) ((l)->free = (m))
#define LIST_RESET_MATCH_METHOD(l, m) ((l)->match = (m))
#define LIST_GET_DUMP_METHOD(l) ((l)->dup)
#define LIST_GET_FREE_METHOD(l) ((l)->free)
#define LIST_GET_MATCH_METHOD(l) ((l)->match)

//链表节点
typedef struct ListNode {

    //前驱节点
    struct ListNode *pre;

    //后驱节点
    struct ListNode *next;

    //节点的值
    void *value;

} listNode;

//链表
typedef struct list {
    //表头节点
    listNode *head;

    //表未节点
    listNode *tail;

    // 节点值复制函数
    void *(*dup)(void *ptr);

    // 节点值释放函数
    void (*free)(void *ptr);

    // 节点值对比函数
    int (*match)(void *ptr, void *key);

    //链表包含的节点数
    unsigned long lens;

} list;

//双端链表的迭代器
typedef struct list_iterator {

    //当前迭代的节点
    listNode *next;

    //迭代的方向
    int direction;

} list_iterator;

//双端链表的常用函数
list *listCreate(void);

void listRelease(list *list);

list *listAddNodeHead(list *list, void *value);

list *listAddNodeTail(list *list, void *value);

list *listInsertNode(list *list, listNode *old_node, void *value, int after);

void listDelNode(list *list, listNode *node);

list_iterator *listGetIterator(list *list, int direction);

listNode *listNext(list_iterator *iter);

void listReleaseIterator(list_iterator *iter);

list *listDup(list *orig);

listNode *listSearchKey(list *list, void *key);

listNode *listIndex(list *list, long index);

void listRewindHead(list *list, list_iterator *iterator);

void listRewindTail(list *list, list_iterator *iterator);

void listRotate(list *list);

#endif //ATHENA_LIST_H


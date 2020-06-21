#include <stdio.h>
#include "t_list/list.h"


int main() {
    printf("%s\n", "main starting....");

    //双端链表的创建
//    list *l = listCreate();
//    printf("%lu\n", sizeof(*l));

    //双端链表的创建与销毁
//    list *l = listCreate();
//    printf("%lu\n", sizeof(*l));
//    listRelease(l);
//    printf("%lu\n", sizeof(*l));

    //测试是插入
    list *l = listCreate();
    int i = 0;
    for (; i < 20; i++) {
        if (i == 10) {
            listAddNodeHead(l, &i);
        } else {
            listAddNodeTail(l, &i);
        }
    }

    list_iterator *iterator = listGetIterator(l, 1);
    while (listNext(iterator) != NULL) {
        listNode *curr = iterator->next;
        iterator->next = curr->next;
        printf("%d\t", *((int *) curr->value));
    }
    printf("\n");


    printf("%s\n", "main ending....");
    return 0;
}

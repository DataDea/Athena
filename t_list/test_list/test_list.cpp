//
// Created by 杨海 on 2020/6/12.
//
#include "/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/include/stdio.h"

//测试双端链表的的创建
list *Test_listCreate() {
    return listCreate();
}

//测试释放整个双端链表
list *Test_listRelease() {
    list *list = listCreate();
    printf("%lu", sizeof(*list));
    listRelease(list);
    if (list != NULL) {
        printf("%s", "双端链表被释放完毕");
    }
}
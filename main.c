#include <stdio.h>
#include "t_list/list.h"
#include "t_list/test_list/test_list.cpp"

int main() {

//    list *ret = Test_listCreate();
//    printf("%lu", sizeof(*ret));

    Test_listRelease();
    return 0;
}

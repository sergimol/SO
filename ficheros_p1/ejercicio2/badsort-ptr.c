#include <stdio.h>

typedef struct {
    char data[4096];
    int key;
} item;

item array[] = {
    {"bill", 3},
    {"neil", 4},
    {"john", 2},
    {"rick", 5},
    {"alex", 1},
};

void sort(item *a, int n) {
    int i = 0, j = 0;
    int s = 1;
    //item* p;

    for(; i < n & s != 0; i++) {
        s = 0;
        //p = a;
        j = n-1;
        do {
            if( a->key > (a+1)->key) {
                item t = *a;
                *a  = *(a+1);
                *(a+1) = t;
                s++;
            }
            else
                *a  = *(a+1);
        } while ( --j >= 0 );
    }
}

int main() {
    int i;
    sort(array,5);
    for(i = 0; i < 5; i++)
        printf("array[%d] = {%s, %d}\n",
                i, array[i].data, array[i].key);
    return 0;
}

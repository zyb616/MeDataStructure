#include <stdio.h>
#include <stdlib.h>

//  (1)顺序表的初始化 (2)顺序表的销毁 (3)顺序表的元素个数 (4)顺序表的插入
//  (5)顺序表的删除 (6)顺序表元素查找 (7)顺序表元素索引 (8)顺序表元素修改
typedef struct
{
    int *elements;   //  一个指向整数数组的指针，用于储存顺序表中的元素
    size_t size;     //  当前顺序表的大小，即元素的个数
    size_t capacity; //  当前顺序表的容量，即数组的大小
} SequentialList;

void SeqListInit(SequentialList *list, int capacity)
{                                                           //  (1)顺序表的初始化
    list->elements = (int *)malloc(sizeof(int) * capacity); //  分配了一个具有指定容量的数组，并将其地址赋予给elemens成员变量
    list->size = 0;                                         //  表示当前顺序表没有元素
    list->capacity = capacity;                              //  记录顺序表最大容量
}

void SeqListDestroy(SequentialList *list)
{                                                           //  (2)顺序表的销毁
    if (list->elements != NULL)
    {                                                       //  释放分配的整数数组内存
        free(list->elements);
        list->elements = NULL;
    }
}

size_t SeqListSize(const SequentialList *list)
{                                                           //  (3)顺序表的元素个数
    return list->size;
}

void SeqListInsert(SequentialList *list, int index, int element)
{                                                                        //  (4)顺序表的插入
    size_t i = 0;                                                        //  注意：这里的index是位置(索引)的意思，可以为0
    if (index < 0 || index > list->size)
    {                                                                    //  如果index小于0或者大于size的话，索引无效，并返回
        printf("Invalid index.\n");
        return;
    }
    if (list->size == list->capacity)
    {                                                                 //  如果顺序表已满(size等于capacity),进行扩容操作
        int *newElements = (int *)realloc(list->elements, sizeof(int) * list->capacity * 2); //  计算新的容量，并通过realloc分配一个新的整数数组
        if (newElements == NULL)
        {
            printf("Faild to allocate memory.\n");
        }
        list->elements = newElements;                                                         //  将顺序表的首地址指向新的数组
        list->capacity = list->capacity * 2;
    }
    for (i = list->size; i > index; i--)
    {                                                                           //  将index(包括index)后的元素依次向后移动
        list->elements[i] = list->elements[i - 1];
    }
    list->elements[index] = element;                                            //  新元素插到index位置
    list->size++;                                                               // 顺序表大小加一
}

void SeqListDelect(SequentialList *list, int index)
{                                                                                // (5)顺序表的删除
    size_t i = 0;
    if (index < 0 || index >= list->size)
    { //  如果index小于0或者大于等于size的话，索引无效，并返回
        printf("Invalid index.\n");
        return;
    }
    for (i = index; i < list->size - 1; i++)
    { //  将index之后的元素向前覆盖,这里可以减一也可以不减，因为最后size-1直接覆盖了
        list->elements[i] = list->elements[i + 1];
    }
    list->size--; //  元素数量减一
}

int SeqListFind(const SequentialList *list, int element)
{ //  (6)顺序表元素查找
    size_t i = 0;
    for (i = 0; i < list->size; i++)
    { //  索引从0到size-1,开始匹配是否等于
        if (list->elements[i] == element)
        {
            return i;
        }
    }
    return -1;
}

int SeqListIndex(const SequentialList *list, int index)
{ //  (7)顺序表元素索引
    size_t i = 0;
    if (index < 0 || index >= list->size)
    { //  检查索引是否有效
        printf("Invalid index\n");
        return -1;
    }
    return list->elements[index];
}

void SeqListSet(SequentialList *list, int index, int element)
{ // (8)顺序表元素修改
    if (index < 0 || index >= list->size)
    {
        printf("Invalid index\n");
        return;
    }
    list->elements[index] = element;
}
int main(void)
{
    SequentialList list;
    SeqListInit(&list, 1);
    for (int i = 0; i < 10; i++)
    {
        SeqListInsert(&list, i, i * 10);
    }

    printf("Size: %lu\n", SeqListSize(&list));

    int elem = SeqListIndex(&list, 2);
    printf("Element found(%d) in index(%d)\n", elem, 2);

    int index = SeqListFind(&list, 15);
    if (index != -1)
    {
        printf("Element found at index %d\n", index);
    }
    else
    {
        printf("Element not found\n");
    }

    // SeqListSet(&list,3,60);
    SeqListDelect(&list, 5);
    printf("Update list:\n");
    for (size_t i = 0; i < list.size; i++)
    {
        printf("%d ", list.elements[i]);
    }
    SeqListDestroy(&list);
    return 0;
}
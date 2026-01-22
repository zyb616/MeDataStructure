#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// 定义向量结构体
typedef struct {
  int *elements;    // 指向动态数组的指针
  size_t capacity;  // 当前分配的内存大小
  size_t size;      // 当前元素数量
} Vector;

// 1.初始化向量
void Vector_Init(Vector *v){
  v->elements = NULL;
  v->capacity = 0;
  v->size = 0;
}

// 2.销毁向量
void Vector_Destroy(Vector *v){
  free(v->elements);
  v->elements = NULL;
  v->capacity = 0;
  v->size = 0;
}

// 3.扩展数组容量
void Vector_Reserve(Vector *v,size_t newCapacity){
  if(newCapacity > v->capacity){
    int *newElements = (int *)malloc(sizeof(int) * newCapacity);
    if(!newElements) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
    }
    //拷贝旧数据
    if(v->elements != NULL){
      memcpy(newElements,v->elements,sizeof(int)*v->size);
      free(v->elements);
    }
    v->elements = newElements;
    v->capacity = newCapacity;
  }
}

//4.添加元素到数组末尾
void Vector_PushBack(Vector *v,int value){
  if(v->size==v->capacity){
    size_t newCap = (v->capacity==0) ? 1 : 2*v->capacity;
    Vector_Reserve(v, newCap);
  }
  v->elements[v->size++] = value;
}

// 5.获取数组中元素的数量
size_t Vector_GetSize(const Vector *v){
  return v->size;
}

// 6.获取数组的容量
size_t Vector_GetCapacity(const Vector *v){
  return v->capacity;
}

// 7.访问数组中的元素 (不进行边界检查的版本，为了性能)
int Vector_Get(const Vector *v, size_t index) {
    return v->elements[index];
}

// 8.在指定位置插入元素
void Vector_Insert(Vector *v,size_t index, int value){
  if(index < 0 || index > v->size){
    // C语言没有异常，这里打印错误并返回
      fprintf(stderr, "Index out of range\n");
      return;
  }
  if(v->size==v->capacity){
    size_t newCap = (v->capacity == 0) ? 1 : v->capacity * 2;
    Vector_Reserve(v, newCap);
  }
  //从后往前移动元素
  for(size_t i = v->size;i>index;--i){
    v->elements[i] = v->elements[i-1];
  }
  v->elements[index] = value;
  v->size++;
}

// 9.删除数组末尾的元素
void Vector_PopBack(Vector *v){
  if(v->size > 0){
    v->size--;
  }
}

// 10.清空数组
void Vector_Clear(Vector *v){
  v->size = 0;
}

// 11.带边界检查的元素访问
int Vector_GetSafe(const Vector *v, size_t index) {
    if (index >= v->size) {
        fprintf(stderr, "Error: Index %zu out of bounds (size=%zu)\n", index, v->size);
        exit(EXIT_FAILURE); // 或返回默认值如 0
    }
    return v->elements[index];
}

// 12.查找元素（返回首次出现的索引）
size_t Vector_Find(const Vector *v,int value){
  for(size_t i=0; i<v->size;++i){
    if(v->elements[i]==value){
      return i;
    }
  }
  return (size_t)-1;
}

// 13.删除指定位置的元素
void Vector_Erase(Vector *v,size_t index){
  if (index >= v->size) {
        fprintf(stderr, "Error: Erase index %zu out of range\n", index);
        return;
    }
    // 将 index+1 到末尾的元素前移
    for (size_t i = index; i < v->size - 1; ++i) {
        v->elements[i] = v->elements[i + 1];
    }
    v->size--;
}

// 14.判断是否为空
int Vector_Empty(const Vector *v){
  return v->size==0;
}

// 15.修改指定位置的元素（带边界检查）
void Vector_Set(Vector *v, size_t index, int value) {
    if (index >= v->size) {
        fprintf(stderr, "Error: Set index %zu out of range\n", index);
        return;
    }
    v->elements[index] = value;
}

// 16.批量添加元素（从另一个数组）
void Vector_AppendArray(Vector *v,const int *arr,size_t count){
  if(!arr||count==0){return;}
  //确保容量足够
  if(v->size +count >v->capacity){
    size_t newCap = v->capacity;
    while (newCap < v->size + count) {
        newCap = (newCap == 0) ? 1 : newCap * 2;
    }
    Vector_Reserve(v, newCap);
  }
  memcpy(v->elements + v->size, arr, sizeof(int) * count);
  v->size += count;
}

// 17,18 Begin,End模拟
int* Vector_Begin(Vector* v) { return v->elements; }
int* Vector_End(Vector* v) { return v->elements + v->size; }

// 19.辅助函数（测试Begin、End）
void PrintVector(const char* label, Vector* v) {
    printf("%s: [", label);
    for (int* p = Vector_Begin(v); p != Vector_End(v); ++p) {
        if (p != Vector_Begin(v)) printf(", ");
        printf("%d", *p);
    }
    printf("] (size=%zu, cap=%zu)\n", Vector_GetSize(v), Vector_GetCapacity(v));
}

int main() {
    Vector v;
    
    // 1. 初始化
    Vector_Init(&v);
    printf("=== 初始化后 ===\n");
    PrintVector("Empty vector", &v);
    printf("Is empty? %s\n", Vector_Empty(&v) ? "Yes" : "No");

    // 2. PushBack 测试（触发扩容）
    printf("\n=== PushBack 测试 ===\n");
    for (int i = 1; i <= 5; ++i) {
        Vector_PushBack(&v, i * 10);
    }
    PrintVector("After PushBack 10,20,30,40,50", &v);

    // 3. Get / GetSafe / Set 测试
    printf("\n=== 访问与修改测试 ===\n");
    printf("Element at index 2: %d\n", Vector_Get(&v, 2));
    printf("Element at index 2 (safe): %d\n", Vector_GetSafe(&v, 2));
    Vector_Set(&v, 2, 999);
    PrintVector("After setting index 2 to 999", &v);

    // 4. Insert 测试
    printf("\n=== Insert 测试 ===\n");
    Vector_Insert(&v, 1, 888); // 在索引1插入888
    PrintVector("After inserting 888 at index 1", &v);

    // 5. Find 测试
    printf("\n=== Find 测试 ===\n");
    size_t idx = Vector_Find(&v, 888);
    if (idx != (size_t)-1) {
        printf("Found 888 at index %zu\n", idx);
    } else {
        printf("888 not found\n");
    }

    // 6. Erase 测试
    printf("\n=== Erase 测试 ===\n");
    Vector_Erase(&v, 0); // 删除第一个元素
    PrintVector("After erasing index 0", &v);

    // 7. PopBack 测试
    printf("\n=== PopBack 测试 ===\n");
    Vector_PopBack(&v);
    PrintVector("After PopBack", &v);

    // 8. AppendArray 测试
    printf("\n=== AppendArray 测试 ===\n");
    int arr[] = {100, 200, 300};
    Vector_AppendArray(&v, arr, 3);
    PrintVector("After appending [100,200,300]", &v);

    // 9. Clear 测试
    printf("\n=== Clear 测试 ===\n");
    Vector_Clear(&v);
    PrintVector("After Clear", &v);
    printf("Is empty? %s\n", Vector_Empty(&v) ? "Yes" : "No");

    // 10. 重新填充并测试 Begin/End 遍历
    printf("\n=== Begin/End 遍历测试 ===\n");
    Vector_PushBack(&v, 7);
    Vector_PushBack(&v, 8);
    Vector_PushBack(&v, 9);
    printf("Using Begin/End loop:\n");
    for (int* p = Vector_Begin(&v); p != Vector_End(&v); ++p) {
        printf("%d ", *p);
    }
    printf("\n");

    // 11. 越界测试（应报错并退出）
    printf("\n=== 越界访问测试（将触发 exit）===\n");
    // 注释掉下面一行可避免程序退出，用于观察其他测试
    // Vector_GetSafe(&v, 100); // 这会触发 "Index out of bounds" 并 exit

    // 如果你想继续运行而不退出，可以改用不安全版本观察行为（不推荐）
    // printf("Unsafe access (UB!): %d\n", Vector_Get(&v, 100));

    // 12. 销毁
    Vector_Destroy(&v);
    printf("\n=== Vector destroyed ===\n");

    return 0;
}
#include "FSPMemoryInstance.h"
#include "stdlib.h"


void * FSPMemoryInstance::allocate(size_t size) {
    return malloc(size);
}
void * FSPMemoryInstance::reallocate(void * pointer, size_t oldSize, size_t size) {
    return realloc(pointer, size);
}
void * FSPMemoryInstance::reallocate(void * pointer, size_t baseTypeSize, size_t oldSize, size_t newSize){
    return reallocate(pointer, baseTypeSize * oldSize, baseTypeSize * newSize);
}
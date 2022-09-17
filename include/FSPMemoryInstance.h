#include "FSPMemoryInterface.h"

class FSPMemoryInstance : public FSPMemoryInterface {
public:
    void * allocate(size_t size) override;
    void * reallocate(void * pointer, size_t oldSize, size_t size) override;
    void * reallocate(void * pointer, size_t baseTypeSize, size_t oldSize, size_t newSize) override;
};
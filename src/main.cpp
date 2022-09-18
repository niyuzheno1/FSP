#include "FSP/export.h"
#include "StringSerializable.h"
#include "MMS/export.h"
using namespace MMS;
SimpleMemoryManageInstance memoryInstance;
SophiscatedMMInstance _memoryInstance;
char tmpx[100];
size_t tsz;
#include "assert.h"
#include "Node.h"
char ttmp[100 * 1024 * 1024];
GenericMemoryManager * gmm;
void Serializable::initializeAllKnownSerializableClasses()
{
    _memoryInstance.setInit([&](void **x){
        *x = ttmp;
    });
    _memoryInstance.memInit();
    gmm = &_memoryInstance;
    Serializable::setMemoryInterface(gmm);
    // Serializable::setLogger(&logger);

    // register serializable
    Serializable *serializable = gmm->newInstance<Serializable>();
    serializable->vSerializableType = registerSerializable(vSerializableType, serializable);

    // register SString
    SString *sString = gmm->newInstance<SString>();
    sString->vSStringType = registerSerializable(SString::vSStringType, sString);

    Node * node = gmm->newInstance<Node>();
    node->vNodeType = registerSerializable(Node::vNodeType, node);
}

void serializationToTmpx(size_t &tsz, const void *_c, size_t sz)
{
    unsigned char *c = (unsigned char *)_c;
    for (size_t i = 0; i < sz; i++)
    {
        tmpx[tsz++] = c[i];
    }
}

void deserializationFromTmpx(size_t &tsz, size_t &led, void *_c, size_t sz)
{
    unsigned char *c = (unsigned char *)_c;

    if (led >= tsz)
    {
        for (size_t i = 0; i < sz; i++)
        {
            c[i] = 0xff;
        }
        return;
    }
    for (size_t i = 0; i < sz; i++)
    {
        if (led >= tsz)
        {
            c[i] = 0xff;
            continue;
        }
        c[i] = tmpx[led++];
    }
}

void run1()
{
    Serializable tmp;
    tsz = 0;
    tmp.serialize([&](const void *_c, size_t sz){
               serializationToTmpx(tsz, _c, sz);
                });
    size_t led = 0;
    size_t cnt = 0;
    Serializable **allSeriable = Serializable::deserializeProcess(
        [&](void *_c, size_t sz)
        {
            deserializationFromTmpx(tsz, led, _c, sz);
        },
        cnt);
    // if (allSeriable == nullptr)
    // {
    //     return;
    // }
    assert(allSeriable != nullptr);
    assert(allSeriable[0]->getType() == Serializable::vSerializableType);
}

void run2()
{
    SString tmp;
    tmp.setString("Hello World");
    tsz = 0;
    tmp.serialize([&](const void *_c, size_t sz)
                  {
                    serializationToTmpx(tsz, _c, sz);
        });
    size_t led = 0;
    size_t cnt = 0;
    Serializable **allSeriable = Serializable::deserializeProcess(
        [&](void *_c, size_t sz)
        {
            deserializationFromTmpx(tsz, led, _c, sz);
        },
        cnt);
    assert(allSeriable != nullptr);
    SString *sString = (SString *)allSeriable[0];
    assert(sString->getString() != nullptr);
    assert(strcmp(sString->getString(), "Hello World") == 0);
}

void run3(){
    Node node1; 
    Node node2;
    node1.setNext(&node2);
    node2.setNext(&node1);
    tsz = 0;
    Serializable * allSerializable[2];
    allSerializable[0] = &node1;
    allSerializable[1] = &node2;
    Serializable::serializeProcess(allSerializable, 2, [&](const void * _c, size_t sz){
        serializationToTmpx(tsz, _c, sz);
    });
    size_t led = 0;
    size_t cnt = 0;
    Serializable **allSeriable = Serializable::deserializeProcess(
        [&](void *_c, size_t sz)
        {
            deserializationFromTmpx(tsz, led, _c, sz);
        },
        cnt);
    assert(allSeriable != nullptr);
    assert(cnt == 2);
    assert(allSeriable[0]->getType() == Node::vNodeType);
    assert(allSeriable[1]->getType() == Node::vNodeType);
    Node * node1_ = (Node *)allSeriable[0];
    Node * node2_ = (Node *)allSeriable[1];
    assert(node1_->getNext() == node2_);
    assert(node2_->getNext() == node1_);
}

void run4(){
    Node node1; 
    SString node2;
    node1.setNext(&node2);
    node2.setString("Hello World");
    tsz = 0;
    Serializable * allSerializable[2];
    allSerializable[0] = &node1;
    allSerializable[1] = &node2;
    Serializable::serializeProcess(allSerializable, 2, [&](const void * _c, size_t sz){
        serializationToTmpx(tsz, _c, sz);
    });
    size_t led = 0;
    size_t cnt = 0;
    Serializable **allSeriable = Serializable::deserializeProcess(
        [&](void *_c, size_t sz)
        {
            deserializationFromTmpx(tsz, led, _c, sz);
        },
        cnt);
    assert(allSeriable != nullptr);
    assert(cnt == 2);
    assert(allSeriable[0]->getType() == Node::vNodeType);
    assert(allSeriable[1]->getType() == SString::vSStringType);
    Node * node1_ = (Node *)allSeriable[0];
    SString * node2_ = (SString *)allSeriable[1];
    assert(node1_->getNext() == node2_);
    assert(strcmp(node2_->getString(), "Hello World") == 0);

}

int main()
{
    Serializable::initializeAllKnownSerializableClasses();
    run1();
    run2();
    run3();
    run4();
    return 0;
}
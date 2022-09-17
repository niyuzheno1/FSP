#include "FSP/export.h"
#include "FSPMemoryInstance.h"
#include "StringSerializable.h"
FSPMemoryInstance memoryInstance;
char tmpx[100];
size_t tsz;
#include "assert.h"

void Serializable::initializeAllKnownSerializableClasses()
{
    Serializable::setMemoryInterface(&memoryInstance);
    // Serializable::setLogger(&logger);

    // register serializable
    Serializable *serializable = memoryInstance.newInstance<Serializable>();
    serializable->vSerializableType = registerSerializable(vSerializableType, serializable);

    // register SString
    SString *sString = memoryInstance.newInstance<SString>();
    sString->vSStringType = registerSerializable(SString::vSStringType, sString);
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
    if (allSeriable == nullptr)
    {
        return;
    }
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
int main()
{
    Serializable::initializeAllKnownSerializableClasses();
    run2();
    return 0;
}
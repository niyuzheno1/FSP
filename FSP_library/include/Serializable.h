#pragma once
#include <functional>
#include "LoggerProto.h"

#include "MMS/export.h"

//Note: This is c++11 feature and is not supported by all compilers
//Note: simulator's writing into the file depends on the endianness of the machine
using InputSimulator = std::function<void(void *, size_t)>;
using OutputSimulator = std::function<void(const void *, size_t)>;

//TODO: add a class for memory allocation

class Serializable {
public:

    //queue for storing the pointers to the point of objects that are ready to be serialized
    static Serializable ** mQueue;
    static size_t headOfQueue;
    static size_t tailOfQueue;
    static void appendToQueue(const Serializable * from, Serializable * serializable);

    static size_t pointValueCount;
    static void ** oldPointerValues;
    static void ** newPointerValues;

    static size_t * serializableIntegerTypes; 
    static Serializable ** serializables;
    static size_t serializableClassCount;

    static size_t observingSerializableCount;
    static Serializable ** observingSerializables;
    static void ** observingSerializableOldPointerValues;

    Serializable();
    static size_t registerSerializable(size_t type, Serializable * serializable);
    static void observe(void * oldPointerValue, Serializable * serializable);
    static void notify(void * oldPointerValue, void * newPointerValue);
    virtual void see(void * oldPointerValue, void * newPointerValue) ; // informed the serialable about the pointer has been read 
    virtual void serialize(const OutputSimulator & os) const;
    virtual bool deserialize(const InputSimulator  & is);
    virtual Serializable * newInstance() const;
    static Serializable ** Serializable::deserializeProcess(const InputSimulator & is, size_t & allSeriablesCount);
    static Serializable * findSerializableByType(int type);
    static void serializeProcess(Serializable ** serializables, size_t allSeriablesCount, const OutputSimulator & os);

    static void * existsOldPointerValue(void * oldPointerValue);
    static void setLogger(LoggerProto * logger);
    static void setMemoryInterface(MMS::GenericMemoryManager * memoryInterface);
    void writePointer(const OutputSimulator & os, void * pointer) const;
    void readPointer(const InputSimulator & is, void * & pointer);
    static void initializeAllKnownSerializableClasses();
    static size_t vSerializableType; 
    size_t getType() const;

protected:
    static LoggerProto * logger;
    size_t mSerializableType;
    static MMS::GenericMemoryManager * memoryInterface;

};